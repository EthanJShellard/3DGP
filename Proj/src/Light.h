#pragma once
#include "glm/glm.hpp"
#include "Transform.h"
#include <memory>

struct Light 
{
	uint32_t ID;
	Transform transform;
	glm::vec3 colour;
	float intensity;

	Light();
	Light(glm::vec3 position, glm::vec3 colour, float intensity);
	~Light();
};