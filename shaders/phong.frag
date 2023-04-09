#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main() {
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 lightDir = normalize(lightPosition - FragPos);
	vec3 normal = normalize(Normal);
	float diffuseValue = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffuseValue * lightColor;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * specularValue * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}