#pragma once
#include "GL/glew.h"
#include <memory>
#include "RenderTexture.h"

class MultisampleRenderTexture
{
private:
	GLuint fbo;
	GLuint rbo;
	GLuint fbt;

	int width;
	int height;

public:
	void Resize(int width, int height);
	void BlitTo(std::shared_ptr<RenderTexture> other);
	void BlitToDefaultFramebuffer();
	void Bind();
	void Unbind();
	GLuint GetTextureID();

	MultisampleRenderTexture(int width, int height, int samples);
	~MultisampleRenderTexture();
};