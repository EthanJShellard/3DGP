#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Transform
{
	glm::quat RotFromTo(glm::vec3 begin, glm::vec3 dest);
};