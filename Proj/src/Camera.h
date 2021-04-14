#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Camera 
{
public:
	Transform transform;
	float vFOV;

	Camera(float vFOV, Transform tf);
	Camera();
};