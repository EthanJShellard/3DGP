#include "CameraController.h"
#include "../Input.h"
#include "../Transform.h"
#include "../Scene.h"

void CameraController::Update(float _deltaTime, std::shared_ptr<Input> _input)
{
	if (auto scPtr = m_scene.lock()) 
	{
		glm::vec3 move = glm::vec3(0);
		//Forward/Backward
		if (_input->GetKey(SDLK_w)) move += glm::vec3(0, 0, -10 * _deltaTime);
		if (_input->GetKey(SDLK_s)) move += glm::vec3(0, 0, 10 * _deltaTime);
		//Left/Right
		if (_input->GetKey(SDLK_a)) move += glm::vec3(-10 * _deltaTime, 0, 0);
		if (_input->GetKey(SDLK_d)) move += glm::vec3(10 * _deltaTime, 0, 0);
		//Speed modifier
		if (_input->GetKey(SDLK_LSHIFT)) move *= 4;

		scPtr->m_mainCamera.m_transform.Translate(scPtr->m_mainCamera.m_transform.GetQuaternionRotation() * move);

		scPtr->m_mainCamera.m_transform.Rotate(-_input->GetMouseDelta().x * _input->m_mouseSensitivity * _deltaTime, glm::vec3(0, 1, 0));
		scPtr->m_mainCamera.m_transform.Rotate(-_input->GetMouseDelta().y * _input->m_mouseSensitivity * _deltaTime, scPtr->m_mainCamera.m_transform.Right());
	}
	
}
