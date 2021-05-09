#pragma once
#include "glm/glm.hpp"
#include "Transform.h"
#include <memory>

struct Light 
{
	uint32_t m_ID;
	Transform m_transform;
	glm::vec3 m_colour;
	float m_intensity;

	Light();
	Light(glm::vec3 _position, glm::vec3 _colour, float _intensity);
	~Light();
};