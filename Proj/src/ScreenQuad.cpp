#include "ScreenQuad.h"
#include "glm/ext.hpp"

ScreenQuad::ScreenQuad(std::shared_ptr<Shader> _shaderProgram, float _windowWidth, float _windowHeight)
{
	//Create buffers
	std::shared_ptr<VertexBuffer> positions = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> texCoords = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> vertNormals = std::make_shared<VertexBuffer>();

	//Fill buffers
	//First tri
	positions->Add(glm::vec3(0, _windowHeight, 0));
	positions->Add(glm::vec3(0, 0, 0));
	positions->Add(glm::vec3(_windowWidth, _windowHeight, 0));
	//Second tri
	positions->Add(glm::vec3(_windowWidth, _windowHeight, 0));
	positions->Add(glm::vec3(0, 0, 0));
	positions->Add(glm::vec3(_windowWidth, 0, 0));
	//First tri
	texCoords->Add(glm::vec2(0, 1));
	texCoords->Add(glm::vec2(0, 0));
	texCoords->Add(glm::vec2(1, 1));
	//Second tri
	texCoords->Add(glm::vec2(1, 1));
	texCoords->Add(glm::vec2(0, 0));
	texCoords->Add(glm::vec2(1, 0));

	//Bind buffers to vao
	m_vao = std::make_shared<VertexArray>();
	m_vao->SetBuffer(positions, 0);
	m_vao->SetBuffer(texCoords, 1);

	//Store buffers so that they are destroyed only when this object is destroyed
	m_buffers.push_back(positions);
	m_buffers.push_back(texCoords);

	//Prepare to use shader
	m_shader = _shaderProgram;
	m_modelLoc = glGetUniformLocation(m_shader->GetID(), "u_Model");
	m_projectionLoc = glGetUniformLocation(m_shader->GetID(), "u_Projection");
	m_viewLoc = glGetUniformLocation(m_shader->GetID(), "u_View");
}

void ScreenQuad::Resize(float _windowWidth, float _windowHeight)
{
	std::shared_ptr<VertexBuffer> newPositions = std::make_shared<VertexBuffer>();
	//First tri
	newPositions->Add(glm::vec3(0, _windowHeight, 0));
	newPositions->Add(glm::vec3(0, 0, 0));
	newPositions->Add(glm::vec3(_windowWidth, _windowHeight, 0));
	//Second tri
	newPositions->Add(glm::vec3(_windowWidth, _windowHeight, 0));
	newPositions->Add(glm::vec3(0, 0, 0));
	newPositions->Add(glm::vec3(_windowWidth, 0, 0));

	m_vao->SetBuffer(newPositions, 0);
	m_buffers.insert(m_buffers.begin(), newPositions);
}

void ScreenQuad::Draw(glm::mat4 _projectionMat)
{
	glBindVertexArray(m_vao->GetID());
	glUseProgram(m_shader->GetID());

	glm::mat4 identity = glm::mat4(1);

	//Generate and upload matrices
	glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(identity));
	glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(_projectionMat));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ScreenQuad::Draw(glm::mat4 _projectionMat, std::shared_ptr<Shader> _shader, GLuint _projectionLoc)
{
	glBindVertexArray(m_vao->GetID());
	glUseProgram(_shader->GetID());

	glUniformMatrix4fv(_projectionLoc, 1, GL_FALSE, glm::value_ptr(_projectionMat));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

