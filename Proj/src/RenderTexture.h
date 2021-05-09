#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <memory>

class PostProcessShader;
class ScreenQuad;

//Wrapper class for a framebuffer object which draws to a texture
class RenderTexture
{
private:
	GLuint m_fbo;
	GLuint m_rbo;
	GLuint m_fbt;

	int m_width;
	int m_height;

public:
	void Resize(int m_width, int m_height);
	void Bind();
	void Unbind();
	GLuint GetTextureID();
	GLuint GetFBOID();

	//Render one RenderTexture into another using a specified shader
	static void RenderFromTo(std::shared_ptr<RenderTexture> _input, std::shared_ptr<RenderTexture> _output, std::shared_ptr<PostProcessShader> _shader, std::shared_ptr<ScreenQuad> _screenQuad, glm::mat4 _projection);
	//Combine two input render textures into an output render texure (Or just use two render textures to draw to a third using whatever shader)
	static void Combine(std::shared_ptr<RenderTexture> _inputA, std::shared_ptr<RenderTexture> _inputB, std::shared_ptr<RenderTexture> _output, std::shared_ptr<PostProcessShader> _shader, std::shared_ptr<ScreenQuad> _screenQuad, glm::mat4 _projection);


	RenderTexture(int m_width, int m_height);
	~RenderTexture();
};