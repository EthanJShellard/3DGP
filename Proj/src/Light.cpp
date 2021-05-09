#include "Light.h"
#include <iostream>

Light::Light()
{
	m_transform.m_position = glm::vec3(0);
	m_colour = glm::vec3(1);
}

Light::Light(glm::vec3 _position, glm::vec3 _colour, float _intensity)
{
	m_transform.m_position = _position;
	m_colour = _colour;
	m_intensity = _intensity;
}

Light::~Light()
{
	std::cout << "Destroying Light\n";
}
