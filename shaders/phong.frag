#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main() {
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// determine if light is pointing in the direction of the normal
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normal = normalize(Normal);
	float dotNL = dot(normal, lightDir);

	vec3 result = ambient;
	if(dotNL > 0.0) {
		// diffuse
		vec3 diffuse = dotNL * lightColor;

		// specular
		float specularStrength = 0.5;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = specularStrength * specularValue * lightColor;

		result = (ambient + diffuse + specular);
	}

	FragColor = vec4(result * objectColor, 1.0);
}