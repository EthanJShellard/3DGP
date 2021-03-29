#include "Transform.h"

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