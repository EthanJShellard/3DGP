#include "Camera.h"
#include <iostream>

void Camera::Update(float deltaTime, std::shared_ptr<Input> input)
{

}

Camera::Camera(float vertFov, float _near, float _far, Transform tf)
{
	vFOV = vertFov;
	transform = tf;
	near = _near;
	far = _far;
}

Camera::Camera()
{
	vFOV = 45.0f;
	near = 0.1f;
	far = 100.0f;
}

Camera::~Camera()
{
	std::cout << "Destroying Camera\n";
}
