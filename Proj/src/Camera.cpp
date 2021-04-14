#include "Camera.h"

Camera::Camera(float vertFov, Transform tf)
{
	vFOV = vertFov;
	transform = tf;
}

Camera::Camera()
{
	vFOV = 90.0f;
}
