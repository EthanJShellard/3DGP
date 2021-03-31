#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "GL/glew.h"
#include "OBJModel.h"
#include <memory>



class GameObject
{
public:
	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetRotation(glm::vec3 newRot);
	void SetRotation(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	void SetScale(float x, float y, float z);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	/// <summary>
	/// Ensure matrices are up to date and perform any update functionality here.
	/// </summary>
	/// <param name="deltaTime">Time since previous frame</param>
	virtual void Update(float deltaTime) = 0;

	void Draw(glm::mat4 perspective, glm::mat4 invView);

protected:
	
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

