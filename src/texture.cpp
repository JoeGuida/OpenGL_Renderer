#include <texture.hpp>

Texture::Texture(const char* filepath, TextureType textureType, ColorChannels colorChannels, TextureWrappingType wrappingType, TextureFilteringType minFilteringType, TextureFilteringType magFilteringType, bool flip) {
	unsigned int texture;
	glGenTextures(1, &texture);

	// texture 1
	glBindTexture(textureType, texture);

	// Set texture wrapping parameters
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrappingType);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrappingType);

	// Set texture filtering parameters
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minFilteringType);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, magFilteringType);

	// Load image, create texture, and generate mipmaps
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(filepath, &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(textureType, 0, colorChannels, width, height, 0, colorChannels, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(textureType);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ID = texture;
}

