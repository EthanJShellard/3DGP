#pragma once
#include "GameObject.h"
#include "OBJModel.h"

class GameObjectOBJ : public GameObject
{
public:
	std::shared_ptr<OBJModel> model;

	/// <summary>
	/// Assign a model read from an OBJ file to this game object.
	/// </summary>
	void SetModel(std::shared_ptr<OBJModel> newModel);

	/// <summary>
	/// Update matrices and draw the renderable components of the object
	/// </summary>
	void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, LightManifest lightManifest) override;
};