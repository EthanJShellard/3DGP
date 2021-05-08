#pragma once
#include "Transform.h"
#include "glm/gtx/transform.hpp"
#include <iostream>

void Transform::SetPosition(glm::vec3 newPos)
{
	position = newPos;
}
void  Transform::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}
void  Transform::SetRotation(glm::vec3 newRot)
{
	orientation = glm::quat(newRot);
}
void  Transform::SetRotation(float x, float y, float z)
{
	orientation = glm::quat(glm::vec3(x, y, z));
}
void Transform::SetRotation(glm::quat newRot)
{
	orientation = newRot;
}
void  Transform::SetScale(glm::vec3 newScale)
{
	scale = newScale;
}
void  Transform::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

void  Transform::Rotate(float angle, glm::vec3 axis)
{
	orientation = glm::angleAxis(glm::radians(angle), axis) * orientation;
}

void Transform::RotateAround(float angle, glm::vec3 axis, glm::vec3 centre)
{
	//Create matrix translating to centre around 0
	glm::mat4 posMat = glm::mat4();
	posMat = glm::translate(posMat, position-centre);
	//Rotate around 0
	posMat = glm::rotate(glm::radians(angle), axis) * posMat;
	//Translate back
	posMat = glm::translate(posMat, centre);
	//Recreate position vector
	position = glm::vec3(posMat * glm::vec4(0,0,0,1));
}

void  Transform::Translate(glm::vec3 move)
{
	position += move;
}

glm::vec3  Transform::GetPosition()
{
	return position;
}
glm::vec3  Transform::GetRotation()
{
	return glm::eulerAngles(orientation);
}
glm::quat Transform::GetQuaternionRotation()
{
	return orientation;
}
glm::vec3  Transform::GetScale()
{
	return scale;
}

glm::vec3 Transform::Right()
{
	return orientation * glm::vec3(1,0,0);
}

glm::vec3 Transform::Up()
{
	return orientation * glm::vec3(0,1,0);
}

glm::vec3 Transform::Forward()
{
	return glm::vec3(0,0,1);
}

glm::quat Transform::RotFromTo(glm::vec3 begin, glm::vec3 dest)
{
	begin = glm::normalize(begin);
	dest = glm::normalize(begin);
	glm::vec3 rotAxis;

	//Dot product gives us cos theta between 2 vectors
	float cosineTheta = dot(begin, dest);

	//In case vectors are in opposite directions.
	//No ideal rotation axis
	//Guess one, get one perpendicular to start
	if (cosineTheta < -1 + 0.001f)
	{
		rotAxis = glm::cross(glm::vec3(.0f, .0f, 1.0f), begin);

		if (glm::length2(rotAxis) < 0.01f) //Parallel, try again
		{
			rotAxis = glm::cross(glm::vec3(1.0f, .0f, .0f), begin);
		}

		rotAxis = glm::normalize(rotAxis);
		return glm::angleAxis(glm::radians(180.0f), rotAxis);
	}

	rotAxis = glm::cross(begin, dest);

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
