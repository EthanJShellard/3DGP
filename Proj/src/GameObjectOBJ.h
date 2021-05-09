#pragma once
#include "GameObject.h"
#include "OBJModel.h"

class GameObjectOBJ : public GameObject
{
public:
	std::shared_ptr<OBJModel> model;

	void SetModel(std::shared_ptr<OBJModel> newModel);

	void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, LightManifest lightManifest) override;
};