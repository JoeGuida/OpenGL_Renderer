#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec2 aTextureCoords;

out vec2 textureCoordinates;

void main() {
	gl_Position = projection * view * model * vec4(localPosition, 1.0f);
	textureCoordinates = aTextureCoords;
}
