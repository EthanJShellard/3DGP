#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "GL/glew.h"
#include <memory>
#include <vector>



class GameObject
{
public:
	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetRotation(glm::vec3 newRot);
	void SetRotation(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	void SetScale(float x, float y, float z);

	void Rotate(float angle, glm::vec3 axis);
	void Translate(glm::vec3 move);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	/// <summary>
	/// Perform any update functionality here.
	/// </summary>
	/// <param name="deltaTime">Time since previous frame</param>
	virtual void Update(float deltaTime);

	/// <summary>
	/// Update matrices and draw the renderable components of the object
	/// </summary>
	virtual void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3>lightPositions);

	GameObject();
protected:
	
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
};

