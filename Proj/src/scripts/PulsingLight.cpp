#include "PulsingLight.h"
#include "../Scene.h"


void PulsingLight::Update(float deltaTs, std::shared_ptr<Input> input)
{
	if (auto lPtr = light.lock()) 
	{
		if (rising) 
		{
			lPtr->intensity += speed * deltaTs;
			if (lPtr->intensity >= maxIntensity) rising = false;
		}
		else 
		{
			lPtr->intensity -= speed * deltaTs;
			if (lPtr->intensity <= minIntensity) rising = true;
		}
	}
}

void PulsingLight::Start()
{
	if (auto scPtr = scene.lock()) 
	{
		std::shared_ptr<Light> lightPtr = std::make_shared<Light>(position, colour, minIntensity);
		scPtr->AddLight(lightPtr);
		light = lightPtr;
	}
}

PulsingLight::PulsingLight(float _minIntensity, float _maxIntensity, float _speed, glm::vec3 _position, glm::vec3 _colour)
{
	minIntensity = _minIntensity;
	maxIntensity = _maxIntensity;
	speed = _speed;
	position = _position;
	colour = _colour;
	rising = true;
}