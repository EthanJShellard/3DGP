#pragma once
#include "GL/glew.h"
#include <memory>
#include "RenderTexture.h"

//A render texture wrapper whose framebuffer texture is multisampled
class MultisampleRenderTexture
{
private:
	GLuint m_fbo;
	GLuint m_rbo;
	GLuint m_fbt;

	int m_width;
	int m_height;
	int m_samples;

public:
	void Resize(int _width, int _height);
	void BlitTo(std::shared_ptr<RenderTexture> _other);
	void BlitToDefaultFramebuffer();
	void Bind();
	void Unbind();
	GLuint GetTextureID();

	MultisampleRenderTexture(int _width, int _height, int _samples);
	~MultisampleRenderTexture();
};