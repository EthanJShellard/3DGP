#include "SpaceshipController.h"
#include "../Input.h"
#include "../Light.h"
#include "../Scene.h"
#include "../GameObjectOBJ.h"

void SpaceshipConrtoller::Update(float deltaTs, std::shared_ptr<Input> input)
{
	glm::vec3 pos = spaceShip->GetPosition();
	float posX = pos.x;

	if (input->GetKey(SDLK_a)) 
	{
		posX = std::max(posX -= speed * deltaTs, leftBound);
	}
	else if (input->GetKey(SDLK_d))
	{
		posX = std::min(posX += speed * deltaTs, rightBound);
	}

	pos.x = posX;

	leftThrusterLight->transform->position = pos + leftThrusterPos;
	rightThrusterLight->transform->position = pos + rightThrusterPos;

	spaceShip->SetPosition(pos);
}

void SpaceshipConrtoller::Start()
{
	spaceShip = scene->FindObjectByID(1);

	leftBound = -3.0f;
	rightBound = 3.0f;
	speed = 3.0f;

	leftThrusterPos = spaceShip->GetScale() * glm::vec3(-1.0f, 0.0f, 3.0f);
	rightThrusterPos = spaceShip->GetScale() * glm::vec3(1.0f, 0.0f, 3.0f);

	leftThrusterLight = std::make_shared<Light>(spaceShip->GetPosition() + leftThrusterPos, glm::vec3(1,0.1,0), .5f);
	rightThrusterLight = std::make_shared<Light>(spaceShip->GetPosition() + rightThrusterPos, glm::vec3(1, 0.1, 0), .5f);

	scene->AddLight(leftThrusterLight);
	scene->AddLight(rightThrusterLight);
}
