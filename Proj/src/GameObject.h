#pragma once
#include "Transform.h"
#include "Input.h"
#include "Light.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "GL/glew.h"
#include <memory>
#include <vector>

class Scene;

class GameObject
{
public:
	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetRotation(glm::vec3 newRot);
	void SetRotation(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	void SetScale(float x, float y, float z);
	void SetScene(std::shared_ptr<Scene> parentScene);

	void Rotate(float angle, glm::vec3 axis);
	void RotateAround(float angle, glm::vec3 axis, glm::vec3 centre);
	void Translate(glm::vec3 move);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	virtual void Start();

	/// <summary>
	/// Perform any update functionality here.
	/// </summary>
	/// <param name="deltaTime">Time since previous frame</param>
	virtual void Update(float deltaTime, std::shared_ptr<Input> input);

	/// <summary>
	/// Update matrices and draw the renderable components of the object
	/// </summary>
	virtual void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<std::shared_ptr<Light> >lightPositions);

	GameObject();

	Uint32 ID = 0;

protected:
	
	glm::mat4 modelMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;

	Transform transform;

	std::shared_ptr<Scene> scene;

	bool dirty = true;

	void UpdateModelMatrix();
};

