#include "CameraController.h"
#include "../Input.h"
#include "../Transform.h"
#include "../Scene.h"

void CameraController::Update(float deltaTime, std::shared_ptr<Input> input)
{
	glm::vec3 move = glm::vec3(0);
	if (input->GetKey(SDLK_w)) move += glm::vec3(0, 0, -10 * deltaTime);//transform.position.z -= 10 * deltaTime;
	if (input->GetKey(SDLK_s)) move += glm::vec3(0, 0, 10 * deltaTime);//transform.position.z += 10 * deltaTime;
	if (input->GetKey(SDLK_a)) move += glm::vec3(-10 * deltaTime, 0, 0);//transform.position.x -= 10 * deltaTime;
	if (input->GetKey(SDLK_d)) move += glm::vec3(10 * deltaTime, 0, 0);//transform.position.x += 10 * deltaTime;
	if (input->GetKey(SDLK_LSHIFT)) move *= 4;
	scene->mainCamera.transform.Translate(scene->mainCamera.transform.GetQuaternionRotation() * move);

	scene->mainCamera.transform.Rotate(-input->GetMouseDelta().x * input->mouseSensitivity * deltaTime, glm::vec3(0, 1, 0));
	scene->mainCamera.transform.Rotate(-input->GetMouseDelta().y * input->mouseSensitivity * deltaTime, scene->mainCamera.transform.Right());
}
