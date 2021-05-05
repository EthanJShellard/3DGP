#include "Camera.h"

void Camera::Update(float deltaTime, std::shared_ptr<Input> input)
{
	//glm::vec3 move = glm::vec3(0);
	//if (input->GetKey(SDLK_w)) move += glm::vec3(0, 0, -10 * deltaTime);//transform.position.z -= 10 * deltaTime;
	//if (input->GetKey(SDLK_s)) move += glm::vec3(0, 0, 10 * deltaTime);//transform.position.z += 10 * deltaTime;
	//if (input->GetKey(SDLK_a)) move += glm::vec3(-10 * deltaTime, 0, 0);//transform.position.x -= 10 * deltaTime;
	//if (input->GetKey(SDLK_d)) move += glm::vec3(10 * deltaTime, 0, 0);//transform.position.x += 10 * deltaTime;
	//if (input->GetKey(SDLK_LSHIFT)) move *= 4;
	//transform.Translate(transform.GetQuaternionRotation() * move);
	//
	//transform.Rotate(-input->GetMouseDelta().x * input->mouseSensitivity, glm::vec3(0,1,0));
	//transform.Rotate(-input->GetMouseDelta().y * input->mouseSensitivity, transform.Right());
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
