#pragma once
#include "../Script.h"
#include "../Light.h"
#include <memory>

class PulsingLight : public Script 
{
	std::weak_ptr<Light> m_light;
	bool m_rising;

public:
	glm::vec3 m_position;
	glm::vec3 m_colour;
	float m_minIntensity;
	float m_maxIntensity;
	float m_speed;

	void Update(float _deltaTs, std::shared_ptr<Input> _input) override;
	void Start() override;

	PulsingLight(float _minIntensity, float _maxIntensity, float _speed, glm::vec3 _position, glm::vec3 _colour);

};