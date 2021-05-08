#pragma once
#include "GL/glew.h"

struct Texture 
{
	GLuint id;

	GLuint CreateTexture(unsigned char* data, int width, int height);
	unsigned char* LoadTextureData(const char* file, int* width, int* height);

	Texture(const char* path);
	~Texture();
};