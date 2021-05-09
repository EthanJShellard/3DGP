#pragma once
#include "GameObject.h"
#include "OBJModel.h"

class GameObjectOBJ : public GameObject
{
public:
	std::shared_ptr<OBJModel> m_model;

	/// <summary>
	/// Assign a model read from an OBJ file to this game object.
	/// </summary>
	void SetModel(std::shared_ptr<OBJModel> _newModel);

	/// <summary>
	/// Update matrices and draw the renderable components of the object
	/// </summary>
	void Draw(glm::mat4 _projection, glm::mat4 _invView, glm::vec3 _camPos, LightManifest _lightManifest) override;
};