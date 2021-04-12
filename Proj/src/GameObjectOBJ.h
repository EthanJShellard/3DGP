#pragma once
#include "GameObject.h"
#include "OBJModel.h"

class GameObjectOBJ : public GameObject
{
private:
	std::shared_ptr<OBJModel> model;
public:
	void SetModel(std::shared_ptr<OBJModel> newModel);

	void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3>lightPositions) override;
};