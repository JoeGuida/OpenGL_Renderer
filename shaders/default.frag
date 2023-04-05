#version 330 core

uniform sampler2D texture0;
uniform sampler2D texture1;

in vec2 textureCoordinates;

out vec4 FragColor;

void main()
{
	FragColor = mix(texture(texture0, textureCoordinates), texture(texture1, textureCoordinates), 0.6);
}