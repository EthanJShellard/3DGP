#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class GameObject
{
public:
	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetRotation(glm::vec3 newRot);
	void SetRotation(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	void SetScale(float x, float y, float z);

	void Update(float deltaTime);
	void Draw(glm::vec4);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
protected:
	glm::mat4 model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

};

