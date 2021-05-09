#include "SpaceshipController.h"
#include "../Input.h"
#include "../Light.h"
#include "../Scene.h"
#include "../GameObjectOBJ.h"

void SpaceshipConrtoller::Update(float _deltaTs, std::shared_ptr<Input> _input)
{
	glm::vec3 pos = m_spaceShip->GetPosition();
	float posX = pos.x;

	//Left-Right movement
	if (_input->GetKey(SDLK_a)) 
	{
		posX = std::max(posX -= m_speed * _deltaTs, m_leftBound);
	}
	else if (_input->GetKey(SDLK_d))
	{
		posX = std::min(posX += m_speed * _deltaTs, m_rightBound);
	}

	pos.x = posX;
	/////////////////////

	//Update light positions
	m_leftThrusterLight->m_transform.m_position = pos + m_leftThrusterPos;
	m_rightThrusterLight->m_transform.m_position = pos + m_rightThrusterPos;

	//Flicker logic
	m_counter += _deltaTs;
	float flicker = std::sin(m_counter);

	glm::vec3 colour = glm::vec3(0.65f - (flicker / 4), 0.1f, 0);
	m_leftThrusterLight->m_intensity = colour.x;
	m_leftThrusterLight->m_colour = colour;
	m_rightThrusterLight->m_intensity = colour.x;
	m_rightThrusterLight->m_colour = colour;
	////////////////

	m_spaceShip->SetPosition(pos);
}

void SpaceshipConrtoller::Start()
{
	if (auto scPtr = m_scene.lock()) 
	{
		//Get spaceship game object
		m_spaceShip = scPtr->FindObjectByID(1);

		m_leftBound = -3.0f;
		m_rightBound = 3.0f;
		m_speed = 3.0f;
		m_counter = 0;

		//Set up light positions
		m_leftThrusterPos = m_spaceShip->GetScale() * glm::vec3(-1.0f, 0.0f, 3.0f);
		m_rightThrusterPos = m_spaceShip->GetScale() * glm::vec3(1.0f, 0.0f, 3.0f);
		//Create lights
		m_leftThrusterLight = std::make_shared<Light>(m_spaceShip->GetPosition() + m_leftThrusterPos, glm::vec3(1, 0.1, 0), .5f);
		m_rightThrusterLight = std::make_shared<Light>(m_spaceShip->GetPosition() + m_rightThrusterPos, glm::vec3(1, 0.1, 0), .5f);

		scPtr->AddLight(m_leftThrusterLight);
		scPtr->AddLight(m_rightThrusterLight);
	}
	
}
