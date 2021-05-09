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
	Transform m_transform;
	float m_vFOV;
	float m_near;
	float m_far;

	void Update(float _deltaTime, std::shared_ptr<Input> _input);

	Camera(float _vFOV, float _near, float _far, Transform _tf);
	Camera();
	~Camera();
};