#include "ScreenQuad.h"
#include "glm/ext.hpp"

ScreenQuad::ScreenQuad(std::shared_ptr<Shader> shaderProgram, float windowWidth, float windowHeight)
{
	//Create buffers
	std::shared_ptr<VertexBuffer> positions = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> texCoords = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> vertNormals = std::make_shared<VertexBuffer>();

	//Fill buffers
	//First tri
	positions->Add(glm::vec3(0, windowHeight, 0));
	positions->Add(glm::vec3(0, 0, 0));
	positions->Add(glm::vec3(windowWidth, windowHeight, 0));
	//Second tri
	positions->Add(glm::vec3(windowWidth, windowHeight, 0));
	positions->Add(glm::vec3(0, 0, 0));
	positions->Add(glm::vec3(windowWidth, 0, 0));
	//First tri
	texCoords->Add(glm::vec2(0, 1));
	texCoords->Add(glm::vec2(0, 0));
	texCoords->Add(glm::vec2(1, 1));
	//Second tri
	texCoords->Add(glm::vec2(1, 1));
	texCoords->Add(glm::vec2(0, 0));
	texCoords->Add(glm::vec2(1, 0));

	//Bind buffers to vao
	vao = std::make_shared<VertexArray>();
	vao->SetBuffer(positions, 0);
	vao->SetBuffer(texCoords, 1);

	//Store buffers so that they are destroyed only when this object is destroyed
	buffers.push_back(positions);
	buffers.push_back(texCoords);

	//Prepare to use shader
	shader = shaderProgram;
	modelLoc = glGetUniformLocation(shader->GetID(), "u_Model");
	projectionLoc = glGetUniformLocation(shader->GetID(), "u_Projection");
	viewLoc = glGetUniformLocation(shader->GetID(), "u_View");
}

void ScreenQuad::Resize(float windowWidth, float windowHeight)
{
	std::shared_ptr<VertexBuffer> newPositions = std::make_shared<VertexBuffer>();
	//First tri
	newPositions->Add(glm::vec3(0, windowHeight, 0));
	newPositions->Add(glm::vec3(0, 0, 0));
	newPositions->Add(glm::vec3(windowWidth, windowHeight, 0));
	//Second tri
	newPositions->Add(glm::vec3(windowWidth, windowHeight, 0));
	newPositions->Add(glm::vec3(0, 0, 0));
	newPositions->Add(glm::vec3(windowWidth, 0, 0));

	vao->SetBuffer(newPositions, 0);
	buffers.insert(buffers.begin(), newPositions);
}

void ScreenQuad::Draw(glm::mat4 projectionMat)
{
	glBindVertexArray(vao->GetID());
	glUseProgram(shader->GetID());

	glm::mat4 identity = glm::mat4(1);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(identity));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ScreenQuad::Draw(glm::mat4 projectionMat, std::shared_ptr<Shader> _shader, GLuint _projectionLoc)
{
	glBindVertexArray(vao->GetID());
	glUseProgram(_shader->GetID());

	glUniformMatrix4fv(_projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

