#ifndef OPENGL_RENDERER_SHADER_HPP
#define OPENGL_RENDERER_SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
};

#endif