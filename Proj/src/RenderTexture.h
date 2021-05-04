#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <memory>

class PostProcessShader;
class ScreenQuad;

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

	static void RenderFromTo(std::shared_ptr<RenderTexture> input, std::shared_ptr<RenderTexture> output, std::shared_ptr<PostProcessShader> shader, std::shared_ptr<ScreenQuad> screenQuad, glm::mat4 projection);
	static void Combine(std::shared_ptr<RenderTexture> inputA, std::shared_ptr<RenderTexture> inputB, std::shared_ptr<RenderTexture> output, std::shared_ptr<PostProcessShader> shader, std::shared_ptr<ScreenQuad> screenQuad, glm::mat4 projection);


	RenderTexture(int width, int height);
	~RenderTexture();
};