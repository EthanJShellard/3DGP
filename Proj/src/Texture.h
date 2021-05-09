#pragma once
#include "GL/glew.h"

//Wrapper struct for an openGL texture
struct Texture 
{
	GLuint id;
	//Create an openGL texture from raw data
	GLuint CreateTexture(unsigned char* data, int width, int height);
	//Load in texture date from the specified file and return a pointer to the first character of the raw data
	unsigned char* LoadTextureData(const char* file, int* width, int* height);

	Texture(const char* path);
	~Texture();
};