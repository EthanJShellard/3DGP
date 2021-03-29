#include "Material.h"
#include <stb_image.h>
#include <exception>

void Material::SetTextureFromFile(const char* path)
{
	if (texture) //If we're replacing the texture
	{
		glDeleteTextures(1, &texture);
	}

	int width;
	int height;

	//Load from file
	unsigned char* data = LoadTextureData(path, &width, &height);
	//Create texture in opengl
	texture = CreateTexture(data, width, height);
}

unsigned char* Material::LoadTextureData(const char* file, int* width, int* height)
{
	unsigned char* data = stbi_load(file, width, height, NULL, 4);

	if (!data) throw std::exception();

	return data;
}

GLuint Material::CreateTexture(unsigned char* data, int width, int height)
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

