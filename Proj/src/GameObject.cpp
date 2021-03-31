#include "GameObject.h"
#include "glm/gtx/quaternion.hpp"

void GameObject::SetPosition(glm::vec3 newPos)
{
	position = newPos;
}
void GameObject::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}
void GameObject::SetRotation(glm::vec3 newRot)
{
	rotation = glm::quat(newRot);
}
void GameObject::SetRotation(float x, float y, float z)
{
	rotation = glm::quat(glm::vec3(x,y,z));
}
void GameObject::SetScale(glm::vec3 newScale)
{
	scale = newScale;
}
void GameObject::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

void GameObject::Rotate(float angle, glm::vec3 axis)
{
	rotation = glm::angleAxis(glm::radians(angle), axis) * rotation;
}

void GameObject::Translate(glm::vec3 move)
{
	position += move;
}

glm::vec3 GameObject::GetPosition()
{
	return position;
}
glm::vec3 GameObject::GetRotation()
{
	return glm::eulerAngles(rotation);
}
glm::vec3 GameObject::GetScale()
{
	return scale;
}

void GameObject::Update(float deltaTime) 
{
}

void GameObject::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3>lightPositions)
{
	//Update model matrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, scale);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::toMat4(rotation) * modelMatrix;

	//Iterate through meshes and draw them with correct materials
	for (int i = 0; i < model->meshes.size(); i++)
	{
		glBindVertexArray(model->meshes.at(i)->vao);
		model->meshes.at(i)->material->Apply(modelMatrix, projection, invView, camPos, lightPositions);
		glDrawArrays(GL_TRIANGLES, 0, model->meshes.at(i)->vertexCount);
	}
}

void GameObject::SetModel(std::shared_ptr<OBJModel> newModel)
{
	model = newModel;
}

GameObject::GameObject()
{
	position = glm::vec3(0);
	scale = glm::vec3(1);
	rotation = glm::quat(glm::vec3(0,0,0));
}

