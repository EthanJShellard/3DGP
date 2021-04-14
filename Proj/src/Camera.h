#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Camera 
{
public:
	Transform transform;
	float vFOV;
	float near;
	float far;

	Camera(float vFOV, Transform tf);
	Camera();
};