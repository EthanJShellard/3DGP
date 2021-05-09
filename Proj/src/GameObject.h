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
	void SetPosition(glm::vec3 _newPos);
	void SetPosition(float _x, float _y, float _z);
	void SetRotation(glm::vec3 _newRot);
	void SetRotation(float _x, float _y, float _z);
	void SetScale(glm::vec3 _newScale);
	void SetScale(float _x, float _y, float _z);
	void SetScene(std::shared_ptr<Scene> _parentScene);

	void Rotate(float _angle, glm::vec3 _axis);
	void Translate(glm::vec3 _move);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	//Called after all objects in a scene are loaded and before the first update call
	virtual void Start();

	//Perform any update functionality here.
	virtual void Update(float _deltaTime, std::shared_ptr<Input> _input);

	//Update matrices and draw the renderable components of the object
	virtual void Draw(glm::mat4 _projection, glm::mat4 _invView, glm::vec3 _camPos, LightManifest _lightManifest);

	GameObject();
	~GameObject();

	Transform m_transform;
	Uint32 m_ID = 0;

protected:
	
	glm::mat4 m_modelMatrix;
	glm::mat4 m_scaleMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_translationMatrix;

	std::weak_ptr<Scene> m_scene;

	bool m_dirty = true;

	//Update this object's model matrix to reflect its position, rotation and scale
	void UpdateModelMatrix();
};

