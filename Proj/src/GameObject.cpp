#include "GameObject.h"

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
	rotation = newRot;
}
void GameObject::SetRotation(float x, float y, float z)
{
	rotation = glm::vec3(x, y, z);
}
void GameObject::SetScale(glm::vec3 newScale)
{
	scale = newScale;
}
void GameObject::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

glm::vec3 GameObject::GetPosition()
{
	return position;
}
glm::vec3 GameObject::GetRotation()
{
	return rotation;
}
glm::vec3 GameObject::GetScale()
{
	return scale;
}

