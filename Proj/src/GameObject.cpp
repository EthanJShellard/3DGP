#include "GameObject.h"
#include "glm/gtx/quaternion.hpp"
#include <iostream>

void GameObject::SetPosition(glm::vec3 newPos)
{
	transform.SetPosition(newPos);
	translationMatrix = glm::translate(glm::mat4(), transform.GetPosition());
	dirty = true;
}
void GameObject::SetPosition(float x, float y, float z)
{
	transform.SetPosition(glm::vec3(x, y, z));
	translationMatrix = glm::translate(glm::mat4(), transform.GetPosition());
	dirty = true;
}
void GameObject::SetRotation(glm::vec3 newRot)
{
	transform.SetRotation(newRot);
	rotationMatrix = glm::toMat4(transform.GetQuaternionRotation());
	dirty = true;
}
void GameObject::SetRotation(float x, float y, float z)
{
	transform.SetRotation(glm::vec3(x,y,z));
	rotationMatrix = glm::toMat4(transform.GetQuaternionRotation());
	dirty = true;
}
void GameObject::SetScale(glm::vec3 newScale)
{
	transform.SetScale(newScale);
	scaleMatrix = glm::scale(glm::mat4(), transform.GetScale());
	dirty = true;
}
void GameObject::SetScale(float x, float y, float z)
{
	transform.SetScale(glm::vec3(x, y, z));
	scaleMatrix = glm::scale(glm::mat4(), transform.GetScale());
	dirty = true;
}

void GameObject::SetScene(std::shared_ptr<Scene> parentScene)
{
	scene = parentScene;
}

void GameObject::Rotate(float angle, glm::vec3 axis)
{
	transform.Rotate(angle, axis);
	rotationMatrix = glm::toMat4(transform.GetQuaternionRotation());
	dirty = true;
}

void GameObject::RotateAround(float angle, glm::vec3 axis, glm::vec3 centre)
{
	transform.RotateAround(angle, axis, centre);
	rotationMatrix = glm::toMat4(transform.GetQuaternionRotation());
	translationMatrix = glm::translate(glm::mat4(), transform.GetPosition());
}

void GameObject::Translate(glm::vec3 move)
{
	transform.Translate(move);
	translationMatrix = glm::translate(glm::mat4(), transform.GetPosition());
	dirty = true;
}

glm::vec3 GameObject::GetPosition()
{
	return transform.GetPosition();
}
glm::vec3 GameObject::GetRotation()
{
	return transform.GetRotation();
}
glm::vec3 GameObject::GetScale()
{
	return transform.GetScale();
}

void GameObject::Start()
{
}

void GameObject::Update(float deltaTime, std::shared_ptr<Input> input)
{
}

void GameObject::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, LightManifest lightManifest)
{
}

GameObject::GameObject()
{
	transform.SetPosition(glm::vec3(0));
	transform.SetScale(glm::vec3(1));
	transform.SetRotation(glm::quat(glm::vec3(0,0,0)));
}

GameObject::~GameObject()
{
	std::cout << "Destroying Gameobject " << ID << std::endl;
}

void GameObject::UpdateModelMatrix()
{
	if (dirty) //If our matrices have been edited
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		dirty = false;
	}
}

