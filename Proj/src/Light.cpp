#include "Light.h"
#include <iostream>

Light::Light()
{
	transform.position = glm::vec3(0);
	colour = glm::vec3(1);
}

Light::Light(glm::vec3 _position, glm::vec3 _colour, float _intensity)
{
	transform.position = _position;
	colour = _colour;
	intensity = _intensity;
}

Light::~Light()
{
	std::cout << "Destroying Light\n";
}
