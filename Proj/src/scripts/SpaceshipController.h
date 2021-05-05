#pragma once
#include "../Script.h"
#include "glm/glm.hpp"

class GameObject;
struct Light;

class SpaceshipConrtoller : public Script 
{
private:
	std::shared_ptr<GameObject> spaceShip;

	float speed;
	float leftBound;
	float rightBound;
	float counter;

	std::shared_ptr<Light> leftThrusterLight;
	glm::vec3 leftThrusterPos;
	std::shared_ptr<Light> rightThrusterLight;
	glm::vec3 rightThrusterPos;

public:
	void Update(float deltaTs, std::shared_ptr<Input> input) override;
	void Start() override;
};