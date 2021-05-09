#include "Texture.h"
#include "stb_image.h"
#include <exception>
#include <iostream>

Texture::Texture(const char* _path)
{
	int width;
	int height;

	//Load from file
	unsigned char* data = LoadTextureData(_path, &width, &height);
	//Create texture in opengl
	m_id = CreateTexture(data, width, height);
}

unsigned char* Texture::LoadTextureData(const char* _file, int* _width, int* _height)
{
	unsigned char* data = stbi_load(_file, _width, _height, NULL, 4);

	if (!data) throw std::exception();

	return data;
}

GLuint Texture::CreateTexture(unsigned char* _data, int _width, int _height)
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);

	//Generate MipMap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

Texture::~Texture()
{
	std::cout << "Destroying texture " << m_id << std::endl;
	glDeleteTextures(1, &m_id);
}
