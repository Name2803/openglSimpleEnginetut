#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 projection;

void main(){
	v_color = color;
	gl_Position = projection * vec4(position, 1.0f);
}