#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <cmath>

#include "graphics/ShaderCl.h"
#include "graphics/Texture.h"
#include "graphics/Mesh.h"
#include "graphics/VoxelRenderer.h"
#include "graphics/LineBatch.h"
#include "window/window.h"
#include "window/Events.h"
#include "window/Camera.h"
#include "voxels/voxel.h"
#include "voxels/Chunk.h"
#include "voxels/Chunks.h"


// settings
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 720;

float vertices[] = {
    //x   //y
    -0.01f, -0.01f,
     0.01f,  0.01f,

    -0.01f,  0.01f,
     0.01f, -0.01f
};


int attrs[] = {
    2, 0//null terminator 
};





int* pup() {
    int a = 4;
    return &a;
}

int main()
{
    int* pp = pup();

    Window::initialize(WIDTH, HEIGHT, "Hi_player)");
    Events::initialize();


    Shader a ("../res/Shader_1.vs", "../res/Shader_1.fs");

    Shader crosshairShadder ("../res/crosshairShader.vs", "../res/crosshairShader.fs");

    Shader lineShader("../res/lineBatchShader.vs", "../res/lineBatchShader.fs");


    Chunks* chunks = new Chunks(4, 1, 4);
    Mesh** meshes = new Mesh * [chunks->volume];
    for (size_t i = 0; i < chunks->volume; i++)
        meshes[i] = nullptr;
    VoxelRenderer renderer(1024 * 1024 * 8);

    LineBatch* lineBatch = new LineBatch(4096);
    
 
    Texture texture1("../texture/texturePuck_1.png");

    a.setInt("Texture", 0);


    Mesh* crosshair = new Mesh(vertices, 4, attrs);
    Camera* camera = new Camera(vec3(0., 5.f, 0.), radians(90.0f));



    glEnable(GL_DEPTH_TEST);


    float lastFrame = static_cast<float>(glfwGetTime());
    float deltaTime;

    float camX = 0.0f;
    float camY = 0.0f;

    while (!Window::isShouldClose()) 
    {
        int speed = 3;

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.6f, 0.62f, 0.65f, 1);

        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }
        if (Events::clicked(GLFW_MOUSE_BUTTON_1)) {
            glClearColor(0.8f, 0.4f, 0.2f, 1);
        }

        if (Events::pressed(GLFW_KEY_W)) {
            camera->position += speed * deltaTime * camera->front;
        }

        if (Events::pressed(GLFW_KEY_S)) {
            camera->position -= speed * deltaTime * camera->front;
        }

        if (Events::pressed(GLFW_KEY_A)) {
            //camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * (float)(speed * deltaTime);
            camera->position -= speed * deltaTime * camera->right;
        }
        if (Events::pressed(GLFW_KEY_D)) {
            camera->position += speed * deltaTime * camera->right;
        }

        if (Events::pressed(GLFW_KEY_SPACE)) {
            camera->position.y += speed * deltaTime * camera->up.y;
        }

        if (Events::pressed(GLFW_KEY_LEFT_SHIFT)) {
            camera->position.y -= speed * deltaTime * camera->up.y;
        }

        if (Events::jpressed(GLFW_KEY_TAB)) {
            Events::toogleCursor();
        }

        //camera->rotate(-Events::deltaY / Window::width, -Events::deltaX / Window::height, 0);

        if (Events::_cursor_locked) {
            camY += -Events::deltaY / Window::height * 2;
            camX += -Events::deltaX / Window::height * 2;

            if (camY < -radians(89.0f)) {
                camY = -radians(89.0f);
            }
            if (camY > radians(89.0f)) {
                camY = radians(89.0f);
            }

            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }


        //isolyated block for ray from camera
        {
            vec3 end;       //end vector
            vec3 norm;      //normal vector(for object which is wached)
            vec3 iend;                                                  //ray langh
            voxel* vox = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
            if (vox != nullptr) {
                if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
                    chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                }
                if (Events::jclicked(GLFW_MOUSE_BUTTON_2)){
                    chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 1);
                }
            }
        }

        
        Chunk* closes[27];
        for (size_t i = 0; i < chunks->volume; i++) {
            Chunk* chunk = chunks->chunks[i];
            //chek if chunk was changed or not
            if (!chunk->modified)
                continue;
            chunk->modified = false;

            //preventing memory leaks
            if (meshes[i] != nullptr)
                delete meshes[i];
            for (int i = 0; i < 27; i++)
                closes[i] = nullptr;
            for (size_t j = 0; j < chunks->volume; j++) {
                Chunk* other = chunks->chunks[j];
                int ox = other->x - chunk->x;
                int oy = other->y - chunk->y;
                int oz = other->z - chunk->z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                    continue;

                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 + oz) * 3 + ox] = other;

            }
            Mesh* mesh = renderer.render(chunks->chunks[i], (const Chunk**)closes, 1);
            meshes[i] = mesh;
        }



        a.use();
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        a.setMat4("projection", camera->getProjection());
        a.setMat4("view", camera->getView());
        texture1.bind();
        a.use();

        for (size_t i = 0; i < chunks->volume; i++) {
            Chunk* chunk = chunks->chunks[i];
            Mesh* mesh = meshes[i];
            model = mat4(1.0f);
            model = glm::translate(model, vec3(chunk->x * CHUNK_W + 0.5, chunk->y * CHUNK_H + 0.5, chunk->z * CHUNK_D + 0.5));
            a.setMat4("model", model);
            mesh->draw(GL_TRIANGLES);
            
        }

        crosshairShadder.use();
        crosshair->draw(GL_LINES);
    
        glm::mat4 projection_1(1.0f);
        projection_1 = camera->getProjection() * camera->getView();


        //lineShader.use();
        //lineShader.uniformMatrix("projection", projection_1);
        //lineBatch->line(
        //    0.0f, 0.0f, 0.0f,
        //    0.0f, 10.0f, 0.0f,

        //    1.0f, 0.0f, 0.0f, 1.0f
        //);
        //lineBatch->render();



        Window::swapBuffers();
        Events::pullEvents();
    }
        delete[] meshes;
        delete chunks;
        delete crosshair;
        //delete lineBatch;
    Window::terminate();
	return 0;
}