#pragma once
#include "MultisampleRenderTexture.h"
#include <exception>

void MultisampleRenderTexture::Resize(int _width, int _height)
{
	width = _width;
	height = _height;

	//Generate new multisampled texure of the new dimensions
	glBindTexture(GL_TEXTURE_2D, fbt);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Adjust the render buffer's storage
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void MultisampleRenderTexture::BlitTo(std::shared_ptr<RenderTexture> other)
{
	//Bind FBOs to respective read and draw framebuffers
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other->GetFBOID());
	//Perform blit
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultisampleRenderTexture::Bind()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void MultisampleRenderTexture::BlitToDefaultFramebuffer()
{
	//Bind fbo as READ framebuffer and default as draw
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//Perform blit
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultisampleRenderTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint MultisampleRenderTexture::GetTextureID()
{
	return fbt;
}

MultisampleRenderTexture::MultisampleRenderTexture(int _width, int _height, int _samples)
{
	width = _width;
	height = _height;
	samples = _samples;

	//Create FrameBufferObject
	fbo = 0;
	glGenFramebuffers(1, &fbo);
	if (!fbo)
	{
		throw std::exception();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Create FrameBufferTexture
	fbt = 0;
	glGenTextures(1, &fbt);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbt);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	//Attach FrameBufferTexture to FrameBufferObject
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbt, 0);

	//Create RenderBufferObject
	rbo = 0;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//Attach RenderBufferObject to FrameBufferObject
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MultisampleRenderTexture::~MultisampleRenderTexture()
{
	glCreateFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &fbt);
}