#pragma once
#include "GL/glew.h"

class RenderTexture
{
private:
	GLuint fbo;
	GLuint rbo;
	GLuint fbt;

	int width;
	int height;

public:
	void Resize(int width, int height);
	void Bind();
	void Unbind();
	GLuint GetTextureID();
	GLuint GetFBOID();

	RenderTexture(int width, int height);
	~RenderTexture();
};