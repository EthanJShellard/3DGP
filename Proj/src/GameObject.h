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
struct LightManifest;

//Generic game object. Intended as superclass.
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

	//Called after all objects in a scene are loaded and before the first update call
	virtual void Start();

	//Perform any update functionality here.
	virtual void Update(float deltaTime, std::shared_ptr<Input> input);

	//Update matrices and draw the renderable components of the object
	virtual void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, LightManifest lightManifest);

	GameObject();
	~GameObject();

	Transform transform;
	Uint32 ID = 0;

protected:
	
	glm::mat4 modelMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;

	std::weak_ptr<Scene> scene;

	bool dirty = true;

	//Update this object's model matrix to reflect its position, rotation and scale
	void UpdateModelMatrix();
};

