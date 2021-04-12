#include "GameObject.h"
#include "glm/gtx/quaternion.hpp"

void GameObject::SetPosition(glm::vec3 newPos)
{
	position = newPos;
	translationMatrix = glm::translate(glm::mat4(), position);
	dirty = true;
}
void GameObject::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	translationMatrix = glm::translate(glm::mat4(), position);
	dirty = true;
}
void GameObject::SetRotation(glm::vec3 newRot)
{
	rotation = glm::quat(newRot);
	rotationMatrix = glm::toMat4(rotation);
	dirty = true;
}
void GameObject::SetRotation(float x, float y, float z)
{
	rotation = glm::quat(glm::vec3(x,y,z));
	rotationMatrix = glm::toMat4(rotation);
	dirty = true;
}
void GameObject::SetScale(glm::vec3 newScale)
{
	scale = newScale;
	scaleMatrix = glm::scale(glm::mat4(), scale);
	dirty = true;
}
void GameObject::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
	scaleMatrix = glm::scale(glm::mat4(), scale);
	dirty = true;
}

void GameObject::Rotate(float angle, glm::vec3 axis)
{
	rotation = glm::angleAxis(glm::radians(angle), axis) * rotation;
	rotationMatrix = glm::toMat4(rotation);
	dirty = true;
}

void GameObject::Translate(glm::vec3 move)
{
	position += move;
	translationMatrix = glm::translate(glm::mat4(), position);
	dirty = true;
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

void GameObject::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3> lightPositions)
{
}

GameObject::GameObject()
{
	position = glm::vec3(0);
	scale = glm::vec3(1);
	rotation = glm::quat(glm::vec3(0,0,0));
}

void GameObject::UpdateModelMatrix()
{
	if (dirty)
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		dirty = false;
	}
}

