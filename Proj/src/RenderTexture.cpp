#include "RenderTexture.h"
#include "PostProcessShader.h"
#include "ScreenQuad.h"
#include <exception>

void RenderTexture::Resize(int _width, int _height)
{
	width = _width;
	height = _height;

	glBindTexture(GL_TEXTURE_2D, fbt);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTexture::Bind()
{
	glViewport(0,0,width,height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void RenderTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint RenderTexture::GetTextureID()
{
	return fbt;
}

GLuint RenderTexture::GetFBOID()
{
	return fbo;
}

void RenderTexture::RenderFromTo(std::shared_ptr<RenderTexture> input, std::shared_ptr<RenderTexture> output, std::shared_ptr<PostProcessShader> shader, std::shared_ptr<ScreenQuad> screenQuad , glm::mat4 projection)
{
	shader->BindTextures(input, input);
	output->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenQuad->Draw(projection, shader, shader->projectionLoc);
	output->Unbind();
}

void RenderTexture::Combine(std::shared_ptr<RenderTexture> inputA, std::shared_ptr<RenderTexture> inputB, std::shared_ptr<RenderTexture> output, std::shared_ptr<PostProcessShader> shader, std::shared_ptr<ScreenQuad> screenQuad, glm::mat4 projection)
{
	shader->BindTextures(inputA, inputB);
	output->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenQuad->Draw(projection, shader, shader->projectionLoc);
	output->Unbind();
}

RenderTexture::RenderTexture(int _width, int _height)
{
	width = _width;
	height = _height;

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
	glBindTexture(GL_TEXTURE_2D, fbt);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Attach FrameBufferTexture to FrameBufferObject
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbt, 0);

	//Create RenderBufferObject
	rbo = 0;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//Attach RenderBufferObject to FrameBufferObject
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


RenderTexture::~RenderTexture()
{
	glCreateFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &fbt);
}
