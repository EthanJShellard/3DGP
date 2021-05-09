#pragma once
#include "../Script.h"
#include "../GameObject.h"

class Spinner : public Script 
{
	std::weak_ptr<GameObject> targetObject;

	float speed;
	glm::vec3 axis;
	int targetID;

public:
	void Update(float deltaTs, std::shared_ptr<Input> input) override;
	void Start() override;
	Spinner(float _speed, glm::vec3 _axis, int _targetID);
};
