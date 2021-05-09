#include "GameObject.h"
#include "glm/gtx/quaternion.hpp"
#include <iostream>

void GameObject::SetPosition(glm::vec3 _newPos)
{
	m_transform.SetPosition(_newPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_transform.GetPosition());
	m_dirty = true;
}
void GameObject::SetPosition(float _x, float _y, float _z)
{
	m_transform.SetPosition(glm::vec3(_x, _y, _z));
	m_translationMatrix = glm::translate(glm::mat4(), m_transform.GetPosition());
	m_dirty = true;
}
void GameObject::SetRotation(glm::vec3 _newRot)
{
	m_transform.SetRotation(_newRot);
	m_rotationMatrix = glm::toMat4(m_transform.GetQuaternionRotation());
	m_dirty = true;
}
void GameObject::SetRotation(float _x, float _y, float _z)
{
	m_transform.SetRotation(glm::vec3(_x,_y,_z));
	m_rotationMatrix = glm::toMat4(m_transform.GetQuaternionRotation());
	m_dirty = true;
}
void GameObject::SetScale(glm::vec3 _newScale)
{
	m_transform.SetScale(_newScale);
	m_scaleMatrix = glm::scale(glm::mat4(), m_transform.GetScale());
	m_dirty = true;
}
void GameObject::SetScale(float _x, float _y, float _z)
{
	m_transform.SetScale(glm::vec3(_x, _y, _z));
	m_scaleMatrix = glm::scale(glm::mat4(), m_transform.GetScale());
	m_dirty = true;
}

void GameObject::SetScene(std::shared_ptr<Scene> _parentScene)
{
	m_scene = _parentScene;
}

void GameObject::Rotate(float _angle, glm::vec3 _axis)
{
	m_transform.Rotate(_angle, _axis);
	m_rotationMatrix = glm::toMat4(m_transform.GetQuaternionRotation());
	m_dirty = true;
}

void GameObject::Translate(glm::vec3 _move)
{
	m_transform.Translate(_move);
	m_translationMatrix = glm::translate(glm::mat4(), m_transform.GetPosition());
	m_dirty = true;
}

glm::vec3 GameObject::GetPosition()
{
	return m_transform.GetPosition();
}
glm::vec3 GameObject::GetRotation()
{
	return m_transform.GetRotation();
}
glm::vec3 GameObject::GetScale()
{
	return m_transform.GetScale();
}

void GameObject::Start()
{
}

void GameObject::Update(float _deltaTime, std::shared_ptr<Input> _input)
{
}

void GameObject::Draw(glm::mat4 _projection, glm::mat4 _invView, glm::vec3 _camPos, LightManifest _lightManifest)
{
}

GameObject::GameObject()
{
	m_transform.SetPosition(glm::vec3(0));
	m_transform.SetScale(glm::vec3(1));
	m_transform.SetRotation(glm::quat(glm::vec3(0,0,0)));
}

GameObject::~GameObject()
{
	std::cout << "Destroying Gameobject " << m_ID << std::endl;
}

void GameObject::UpdateModelMatrix()
{
	if (m_dirty) //If our matrices have been edited
	{
		m_modelMatrix = glm::mat4(1.0f);
		m_modelMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;

		m_dirty = false;
	}
}

