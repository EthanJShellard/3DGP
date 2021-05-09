#pragma once
#include "../Script.h"
#include "../Light.h"
#include <memory>

class PulsingLight : public Script 
{
	std::weak_ptr<Light> light;
	bool rising;

public:
	glm::vec3 position;
	glm::vec3 colour;
	float minIntensity;
	float maxIntensity;
	float speed;

	void Update(float deltaTs, std::shared_ptr<Input> input) override;
	void Start() override;

	PulsingLight(float _minIntensity, float _maxIntensity, float _speed, glm::vec3 _position, glm::vec3 _colour);

};