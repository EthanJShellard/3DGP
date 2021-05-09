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
	vao =  std::make_shared<VertexArray>();
	vao->SetBuffer(positions, 0);
	vao->SetBuffer(texCoords, 1);
	vao->SetBuffer(vertNormals, 2);

	//Store buffers so they are only destroyed when object goes out of scope.
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
	material->texture = std::make_shared<Texture>(texturePath.c_str());
	material->SetShader(shader);
	BuildArrayObject();
}

LoneQuad::LoneQuad(std::shared_ptr<Texture> tex, std::shared_ptr<Shader> shader)
{
	transform.SetPosition(glm::vec3(0));
	transform.SetScale(glm::vec3(1));
	transform.SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	material = std::make_shared<Material>();
	material->texture = tex;
	material->SetShader(shader);
	BuildArrayObject();
}

void LoneQuad::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, LightManifest lightManifest)
{
	UpdateModelMatrix();
	//Bind vao
	glBindVertexArray(vao->GetID());
	//Apply material, bind shader and upload light information
	material->Apply(modelMatrix, projection, invView, camPos, lightManifest);
	//Draw quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
