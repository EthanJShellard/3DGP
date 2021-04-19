#include "Scene.h"

void Scene::Update(float deltaTime)
{
	mainCamera.Update(deltaTime, input);

	for (int i = 0; i < gameObjects.size(); i++) 
	{
		gameObjects.at(i)->Update(deltaTime, input);
	}
}

void Scene::Draw(float windowWidth, float windowHeight)
{
	//Create perspective projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(mainCamera.vFOV),
		windowWidth / windowHeight, mainCamera.near, mainCamera.far);

	//Create view matrix
	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, mainCamera.transform.GetPosition()) * glm::toMat4(mainCamera.transform.GetQuaternionRotation()) * view;	

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->Draw(projection, glm::inverse(view), mainCamera.transform.GetPosition(), lights);
	}
}

void Scene::AddObject(std::shared_ptr<GameObject> go)
{
	gameObjects.push_back(go);
}

void Scene::AddLight(std::shared_ptr<Light> light)
{
	lights.push_back(light);
}

Scene::Scene(std::shared_ptr<Input> _input)
{
	input = _input;
}
