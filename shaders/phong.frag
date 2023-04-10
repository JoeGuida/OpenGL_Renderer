#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

void main() {
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// determine if light is pointing in the direction of the normal
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 normal = normalize(Normal);
	float diff = dot(normal, lightDir);

	vec3 result = ambient;
	if(diff > 0.0) {
		// diffuse
		vec3 diffuse = light.diffuse * (diff * material.diffuse);

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * (spec * material.specular);

		result += (diffuse + specular);
	}

	FragColor = vec4(result, 1.0);
}