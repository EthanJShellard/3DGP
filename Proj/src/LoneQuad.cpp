#include "LoneQuad.h"

void LoneQuad::BuildArrayObject()
{
	//Create buffers
	std::shared_ptr<VertexBuffer> positions = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> texCoords = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> vertNormals = std::make_shared<VertexBuffer>(); 

	//Construct buffer data
	//First tri positions
	positions->Add(glm::vec3(0, 0, 0));
	positions->Add(glm::vec3(0, 0, 1));
	positions->Add(glm::vec3(1, 0, 0));
	//Second tri positions
	positions->Add(glm::vec3(1, 0, 1));
	positions->Add(glm::vec3(1, 0, 0));
	positions->Add(glm::vec3(0, 0, 1));
	//First tri texture coords
	texCoords->Add(glm::vec2(0, 0));
	texCoords->Add(glm::vec2(0, 1));
	texCoords->Add(glm::vec2(1, 0));
	//Second tri texture coords
	texCoords->Add(glm::vec2(1, 1));
	texCoords->Add(glm::vec2(1, 0));
	texCoords->Add(glm::vec2(0, 1));
	//First tri normals
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));
	//Second tri normals	   	 
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));

	//Bind buffers to vao
	m_vao =  std::make_shared<VertexArray>();
	m_vao->SetBuffer(positions, 0);
	m_vao->SetBuffer(texCoords, 1);
	m_vao->SetBuffer(vertNormals, 2);

	//Store buffers so they are only destroyed when object goes out of scope.
	m_buffers.push_back(positions);
	m_buffers.push_back(texCoords);
	m_buffers.push_back(vertNormals);
}

LoneQuad::LoneQuad(std::string _texturePath, std::shared_ptr<Shader> _shader)
{
	m_transform.SetPosition(glm::vec3(0));
	m_transform.SetScale(glm::vec3(1));
	m_transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	m_material = std::make_shared<Material>();
	m_material->m_texture = std::make_shared<Texture>(_texturePath.c_str());
	m_material->SetShader(_shader);
	BuildArrayObject();
}

LoneQuad::LoneQuad(std::shared_ptr<Texture> _tex, std::shared_ptr<Shader> _shader)
{
	m_transform.SetPosition(glm::vec3(0));
	m_transform.SetScale(glm::vec3(1));
	m_transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	m_material = std::make_shared<Material>();
	m_material->m_texture = _tex;
	m_material->SetShader(_shader);
	BuildArrayObject();
}

void LoneQuad::Draw(glm::mat4 _projection, glm::mat4 _invView, glm::vec3 _camPos, LightManifest _lightManifest)
{
	UpdateModelMatrix();
	//Bind vao
	glBindVertexArray(m_vao->GetID());
	//Apply material, bind shader and upload light information
	m_material->Apply(m_modelMatrix, _projection, _invView, _camPos, _lightManifest);
	//Draw quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
