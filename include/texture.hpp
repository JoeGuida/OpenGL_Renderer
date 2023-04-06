#ifndef OPENGL_RENDERER_TEXTURE_HPP
#define OPENGL_RENDERER_TEXTURE_HPP

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <Windows.h>

enum ColorChannels {
	RGB = GL_RGB,
	RGBA = GL_RGBA
};

enum TextureType {
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_3D = GL_TEXTURE_3D
};

enum TextureWrappingType {
	REPEAT = GL_REPEAT,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

enum TextureFilteringType {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

// Helper methods to pass local filepaths for shaders/textures
static std::string getCurrentDirectoryOnWindows()
{
	const unsigned long maxDir = 260;
	char currentDir[maxDir];
	GetCurrentDirectory(maxDir, currentDir);
	return std::string(currentDir);
}

static std::string getLocalFilepath(std::string value) {
	return (getCurrentDirectoryOnWindows() + value);
}

class Texture {

public:
	unsigned int ID;
	Texture(const char* filepath, TextureType textureType, ColorChannels colorChannels, TextureWrappingType wrappingType, TextureFilteringType minFilteringType, TextureFilteringType magFilteringType,  bool flip = true);
};

#endif