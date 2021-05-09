#pragma once
#include "MultisampleRenderTexture.h"
#include <exception>

void MultisampleRenderTexture::Resize(int _width, int _height)
{
	m_width = _width;
	m_height = _height;

	//Generate new multisampled texure of the new dimensions
	glBindTexture(GL_TEXTURE_2D, m_fbt);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, m_width, m_height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Adjust the render buffer's storage
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void MultisampleRenderTexture::BlitTo(std::shared_ptr<RenderTexture> _other)
{
	//Bind FBOs to respective read and draw framebuffers
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _other->GetFBOID());
	//Perform blit
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultisampleRenderTexture::Bind()
{
	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void MultisampleRenderTexture::BlitToDefaultFramebuffer()
{
	//Bind fbo as READ framebuffer and default as draw
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//Perform blit
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultisampleRenderTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint MultisampleRenderTexture::GetTextureID()
{
	return m_fbt;
}

MultisampleRenderTexture::MultisampleRenderTexture(int _width, int _height, int _samples)
{
	m_width = _width;
	m_height = _height;
	m_samples = _samples;

	//Create FrameBufferObject
	m_fbo = 0;
	glGenFramebuffers(1, &m_fbo);
	if (!m_fbo)
	{
		throw std::exception();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//Create FrameBufferTexture
	m_fbt = 0;
	glGenTextures(1, &m_fbt);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_fbt);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, m_width, m_height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	//Attach FrameBufferTexture to FrameBufferObject
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_fbt, 0);

	//Create RenderBufferObject
	m_rbo = 0;
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//Attach RenderBufferObject to FrameBufferObject
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MultisampleRenderTexture::~MultisampleRenderTexture()
{
	glCreateFramebuffers(1, &m_fbo);
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteTextures(1, &m_fbt);
}