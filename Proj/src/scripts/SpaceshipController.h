#pragma once
#include "../Script.h"
#include "glm/glm.hpp"

class GameObject;
struct Light;

class SpaceshipConrtoller : public Script 
{
private:
	std::shared_ptr<GameObject> m_spaceShip;

	float m_speed;
	float m_leftBound;
	float m_rightBound;
	float m_counter;

	std::shared_ptr<Light> m_leftThrusterLight;
	glm::vec3 m_leftThrusterPos;
	std::shared_ptr<Light> m_rightThrusterLight;
	glm::vec3 m_rightThrusterPos;

public:
	void Update(float _deltaTs, std::shared_ptr<Input> _input) override;
	void Start() override;
};