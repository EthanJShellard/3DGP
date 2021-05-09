#include "Texture.h"
#include "stb_image.h"
#include <exception>
#include <iostream>

Texture::Texture(const char* path)
{
	int width;
	int height;

	//Load from file
	unsigned char* data = LoadTextureData(path, &width, &height);
	//Create texture in opengl
	id = CreateTexture(data, width, height);
}

unsigned char* Texture::LoadTextureData(const char* file, int* width, int* height)
{
	unsigned char* data = stbi_load(file, width, height, NULL, 4);

	if (!data) throw std::exception();

	return data;
}

GLuint Texture::CreateTexture(unsigned char* data, int width, int height)
{
	//Create and bind texture
	GLuint texID = 0;
	glGenTextures(1, &texID);

	if (!texID)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, texID);

	//Upload image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//Generate MipMap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

Texture::~Texture()
{
	std::cout << "Destroying texture " << id << std::endl;
	glDeleteTextures(1, &id);
}
