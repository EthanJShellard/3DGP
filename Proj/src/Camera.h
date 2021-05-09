#pragma once
#include "Transform.h"
#include "Input.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <memory>

/// <summary>
/// Class containing camera information such as transform, field of view and, near & far plane distances
/// </summary>
class Camera 
{
public:
	Transform transform;
	float vFOV;
	float near;
	float far;

	void Update(float deltaTime, std::shared_ptr<Input> input);

	Camera(float vFOV, float near, float far, Transform tf);
	Camera();
	~Camera();
};