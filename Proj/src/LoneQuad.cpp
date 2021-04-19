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
	transform.SetPosition(glm::vec3(0));
	transform.SetScale(glm::vec3(1));
	transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	material = std::make_shared<Material>();
	material->SetTextureFromFile(texturePath.c_str());
	material->SetShader(shader);
	BuildArrayObject();
}

LoneQuad::LoneQuad(GLuint texture, std::shared_ptr<Shader> shader)
{
	transform.SetPosition(glm::vec3(0));
	transform.SetScale(glm::vec3(1));
	transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	material = std::make_shared<Material>();
	material->SetTexture(texture);
	material->SetShader(shader);
	BuildArrayObject();
}

void LoneQuad::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector< std::shared_ptr<Light> > lights)
{
	UpdateModelMatrix();

	std::vector<float> lightPositions;
	std::vector<float> lightColours;
	int count = std::min((int)lights.size(), 5);//We have a hard cap of 5 light sources

	for (int i = 0; i < count; i++)
	{
		lightPositions.push_back(lights.at(i)->position.x);
		lightPositions.push_back(lights.at(i)->position.y);
		lightPositions.push_back(lights.at(i)->position.z);

		lightColours.push_back(lights.at(i)->colour.x);
		lightColours.push_back(lights.at(i)->colour.y);
		lightColours.push_back(lights.at(i)->colour.z);
	}

	glBindVertexArray(vao->GetID());
	material->Apply(modelMatrix, projection, invView, camPos, lightPositions, lightColours, count);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
