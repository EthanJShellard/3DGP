#pragma once
#include "../GameObject.h"

class SpinningLight : public GameObject
{
public:
	void Start() override;
	void Update(float deltaTime, std::shared_ptr<Input> input) override;

	std::shared_ptr<Light> light;
	glm::vec3 centre = glm::vec3(10,1,10);
	glm::vec3 axis = glm::vec3(0,1,0);
	float radius = 0.1f;
	float rotationSpeed = 45.0f;
};