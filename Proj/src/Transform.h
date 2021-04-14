#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

struct Transform
{
private:
	glm::vec3 position = glm::vec3(0);
	glm::quat orientation = glm::quat(glm::vec3(0,0,0));
	glm::vec3 scale = glm::vec3(1);
public:
	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetRotation(glm::vec3 newRot);
	void SetRotation(float x, float y, float z);
	void SetRotation(glm::quat newRot);
	void SetScale(glm::vec3 newScale);
	void SetScale(float x, float y, float z);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::quat GetQuaternionRotation();
	glm::vec3 GetScale();

	void Rotate(float angle, glm::vec3 axis);
	void Translate(glm::vec3 move);

	static glm::quat RotFromTo(glm::vec3 begin, glm::vec3 dest);
};