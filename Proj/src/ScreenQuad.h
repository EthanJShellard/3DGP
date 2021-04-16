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
private:
	std::shared_ptr<VertexArray> vao;
	std::vector< std::shared_ptr<VertexBuffer> > buffers;
	std::shared_ptr<Shader> shader;

	GLuint projectionLoc;
	GLuint viewLoc;
	GLuint modelLoc;
};
