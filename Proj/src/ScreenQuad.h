#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "GL/glew.h"

#include <memory>

class ScreenQuad
{
public:
	ScreenQuad(std::shared_ptr<Shader> shader, float windowWidth, float windowHeight);
	void Resize(float windowWidth, float windowHeight);
	void Draw(glm::mat4 projectionMat);
	//Use for drawing from one RenderTexture to another - does not account for model or view matrices
	void Draw(glm::mat4 projectionMat, std::shared_ptr<Shader> shader, GLuint _projectionLoc);

private:
	std::shared_ptr<VertexArray> vao;
	std::vector< std::shared_ptr<VertexBuffer> > buffers;
	std::shared_ptr<Shader> shader;

	GLuint projectionLoc;
	GLuint viewLoc;
	GLuint modelLoc;
};
