#pragma once
#include "SpinningLight.h"
#include <iostream>

void SpinningLight::Start()
{
	light = std::make_shared<Light>(centre + glm::vec3(0,0, -radius), glm::vec3(1, 0, 0));
	scene->AddLight(light);
}

void SpinningLight::Update(float deltaTime, std::shared_ptr<Input> input)
{
	light->transform->RotateAround(rotationSpeed * deltaTime, axis, centre);
}
