#ifndef OPENGL_RENDERER_SHADER_HPP
#define OPENGL_RENDERER_SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	void setFloat(const char* uniformName, float value);
	void setInt(const char* uniformName, int value);
	void setMatrix4f(const char* uniformName, const glm::mat4& value);
	void setVec3(const char* uniformName, const glm::vec3& value);
	void setVec3(const char* uniformName, float x, float y, float z);
	void use();
};

#endif