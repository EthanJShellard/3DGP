#pragma once
#include "../Script.h"
#include "../GameObject.h"

class Spinner : public Script 
{
	std::weak_ptr<GameObject> m_targetObject;

	float m_speed;
	glm::vec3 m_axis;
	int m_targetID;

public:
	void Update(float _deltaTs, std::shared_ptr<Input> _input) override;
	void Start() override;
	Spinner(float _speed, glm::vec3 _axis, int _targetID);
};
