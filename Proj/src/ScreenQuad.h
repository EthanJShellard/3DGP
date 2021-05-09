#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "GL/glew.h"

#include <memory>

//Wrapper for a quad which spans the entire screen - primarily used to draw render textures to the screen.
class ScreenQuad
{
public:
	ScreenQuad(std::shared_ptr<Shader> _shader, float _windowWidth, float _windowHeight);
	void Resize(float _windowWidth, float _windowHeight);
	void Draw(glm::mat4 _projectionMat);

	//Use for drawing from one RenderTexture to another - does not account for model or view matrices
	void Draw(glm::mat4 _projectionMat, std::shared_ptr<Shader> _shader, GLuint _projectionLoc);

private:
	std::shared_ptr<VertexArray> m_vao;
	std::vector< std::shared_ptr<VertexBuffer> > m_buffers;
	std::shared_ptr<Shader> m_shader;

	GLuint m_projectionLoc;
	GLuint m_viewLoc;
	GLuint m_modelLoc;
};
