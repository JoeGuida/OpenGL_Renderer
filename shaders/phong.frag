#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 objectColor;

void main() {
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 lightDirection = normalize(lightPosition - FragPos);
	vec3 normal = normalize(Normal);
	float diffuseValue = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diffuseValue * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}