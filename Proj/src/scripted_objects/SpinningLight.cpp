#pragma once
#include "SpinningLight.h"
#include <iostream>

void SpinningLight::Start()
{
	if (auto scPtr = scene.lock()) 
	{
		std::shared_ptr<Light> lightPtr = std::make_shared<Light>(centre + glm::vec3(0, 0, -radius), glm::vec3(1, 0, 0), 0.5f);
		scPtr->AddLight(lightPtr);
		light = lightPtr;
	}
	
}

void SpinningLight::Update(float deltaTime, std::shared_ptr<Input> input)
{
	if(auto lPtr = light.lock()) lPtr->transform->RotateAround(rotationSpeed * deltaTime, axis, centre);
}
