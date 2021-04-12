#include "LoneQuad.h"

void LoneQuad::BuildArrayObject()
{
	//Create buffers
	std::shared_ptr<VertexBuffer> positions = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> texCoords = std::make_shared<VertexBuffer>();
	std::shared_ptr<VertexBuffer> vertNormals = std::make_shared<VertexBuffer>(); 

	//Fill buffers
	//First tri
	positions->Add(glm::vec3(0, 0, 0));
	positions->Add(glm::vec3(0, 0, 1));
	positions->Add(glm::vec3(1, 0, 0));
	//Second tri
	positions->Add(glm::vec3(1, 0, 1));
	positions->Add(glm::vec3(1, 0, 0));
	positions->Add(glm::vec3(0, 0, 1));
	//First tri
	texCoords->Add(glm::vec2(0, 0));
	texCoords->Add(glm::vec2(0, 1));
	texCoords->Add(glm::vec2(1, 0));
	//Second tri
	texCoords->Add(glm::vec2(1, 1));
	texCoords->Add(glm::vec2(1, 0));
	texCoords->Add(glm::vec2(0, 1));
	//First tri
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));
	//Second tri			   	 
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));
	vertNormals->Add(glm::vec3(0, 1, 0));

	//Bind buffers to vao
	vao =  std::make_shared<VertexArray>();
	vao->SetBuffer(positions, 0);
	vao->SetBuffer(texCoords, 1);
	vao->SetBuffer(vertNormals, 2);

	//Store buffers so they are only destroyed when object goes out of scope. This fixed a problem I was having on amd systems
	buffers.push_back(positions);
	buffers.push_back(texCoords);
	buffers.push_back(vertNormals);
}

LoneQuad::LoneQuad(std::string texturePath, std::shared_ptr<Shader> shader)
{
	position = glm::vec3(0,0,0);
	rotation = glm::quat(glm::vec3(0, 0, 0));
	scale = glm::vec3(1,1,1);

	material = std::make_shared<Material>();
	material->SetTextureFromFile(texturePath.c_str());
	material->SetShader(shader);
	BuildArrayObject();
}

LoneQuad::LoneQuad(GLuint texture, std::shared_ptr<Shader> shader)
{
	position = glm::vec3(0);
	rotation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1);

	material = std::make_shared<Material>();
	material->SetTexture(texture);
	material->SetShader(shader);
	BuildArrayObject();
}

void LoneQuad::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3> lightPositions)
{
	UpdateModelMatrix();

	glBindVertexArray(vao->GetID());
	material->Apply(modelMatrix, projection, invView, camPos, lightPositions);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
