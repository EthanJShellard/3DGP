#pragma once
#include "GL/glew.h"

class RenderTexture
{
private:
	GLuint fbo;
	GLuint rbo;
	GLuint fbt;

public:
	void Resize(int width, int height);
	void Bind();
	void Unbind();
	GLuint GetTextureID();

	RenderTexture(int width, int height);
	~RenderTexture();
};