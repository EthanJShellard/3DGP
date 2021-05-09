#pragma once
#include "Transform.h"
#include "glm/gtx/transform.hpp"
#include <iostream>

void Transform::SetPosition(glm::vec3 _newPos)
{
	m_position = _newPos;
}
void  Transform::SetPosition(float _x, float _y, float _z)
{
	m_position = glm::vec3(_x, _y, _z);
}
void  Transform::SetRotation(glm::vec3 _newRot)
{
	m_orientation = glm::quat(_newRot);
}
void  Transform::SetRotation(float _x, float _y, float _z)
{
	m_orientation = glm::quat(glm::vec3(_x, _y, _z));
}
void Transform::SetRotation(glm::quat _newRot)
{
	m_orientation = _newRot;
}
void  Transform::SetScale(glm::vec3 _newScale)
{
	m_scale = _newScale;
}
void  Transform::SetScale(float _x, float _y, float _z)
{
	m_scale = glm::vec3(_x, _y, _z);
}

void  Transform::Rotate(float _angle, glm::vec3 _axis)
{
	m_orientation = glm::angleAxis(glm::radians(_angle), _axis) * m_orientation;
}

void  Transform::Translate(glm::vec3 _move)
{
	m_position += _move;
}

glm::vec3  Transform::GetPosition()
{
	return m_position;
}
glm::vec3  Transform::GetRotation()
{
	return glm::eulerAngles(m_orientation);
}
glm::quat Transform::GetQuaternionRotation()
{
	return m_orientation;
}
glm::vec3  Transform::GetScale()
{
	return m_scale;
}

glm::vec3 Transform::Right()
{
	return m_orientation * glm::vec3(1,0,0);
}

glm::vec3 Transform::Up()
{
	return m_orientation * glm::vec3(0,1,0);
}

glm::vec3 Transform::Forward()
{
	return m_orientation * glm::vec3(0,0,1);
}

glm::quat Transform::RotFromTo(glm::vec3 _begin, glm::vec3 _dest)
{
	_begin = glm::normalize(_begin);
	_dest = glm::normalize(_begin);
	glm::vec3 rotAxis;

	//Dot product gives us cos theta between 2 vectors
	float cosineTheta = dot(_begin, _dest);

	//In case vectors are in opposite directions.
	//No ideal rotation axis
	//Guess one, get one perpendicular to start
	if (cosineTheta < -1 + 0.001f)
	{
		rotAxis = glm::cross(glm::vec3(.0f, .0f, 1.0f), _begin);

		if (glm::length2(rotAxis) < 0.01f) //Parallel, try again
		{
			rotAxis = glm::cross(glm::vec3(1.0f, .0f, .0f), _begin);
		}

		rotAxis = glm::normalize(rotAxis);
		return glm::angleAxis(glm::radians(180.0f), rotAxis);
	}

	rotAxis = glm::cross(_begin, _dest);

	float s = sqrt((1 + cosineTheta) * 2);
	float inverse = 1 / s;

	return glm::quat
	{
		s * 0.5f,
		rotAxis.x * inverse,
		rotAxis.y * inverse,
		rotAxis.z * inverse
	};
}

Transform::~Transform()
{
	std::cout << "Destroying Transform\n";
}
