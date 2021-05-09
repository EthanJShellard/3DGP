#include "Scene.h"
#include <iostream>

void Scene::Start()
{
	//Initialise game objects
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->Start();
	}
	//Initialise scripts
	for (int i = 0; i < scripts.size(); i++) 
	{
		scripts.at(i)->Start();
	}
}

void Scene::Update(float deltaTime)
{
	//Update camera
	mainCamera.Update(deltaTime, input);

	//Update game objects
	for (int i = 0; i < gameObjects.size(); i++) 
	{
		gameObjects.at(i)->Update(deltaTime, input);
	}

	//Update scripts
	for (int i = 0; i < scripts.size(); i++)
	{
		scripts.at(i)->Update(deltaTime, input);
	}

	//Update light manifest
	lightManifest.Update(lights);
}

void Scene::Draw(float windowWidth, float windowHeight)
{
	//Create perspective projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(mainCamera.vFOV),
		windowWidth / windowHeight, mainCamera.near, mainCamera.far);

	//Create view matrix
	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, mainCamera.transform.GetPosition()) * glm::toMat4(mainCamera.transform.GetQuaternionRotation()) * view;	

	//Draw each game object
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->Draw(projection, glm::inverse(view), mainCamera.transform.GetPosition(), lightManifest);
	}
}

void Scene::AddObject(std::shared_ptr<GameObject> go)
{
	go->SetScene(shared_from_this()); //Provide object reference to this scene
	gameObjects.push_back(go);
}

void Scene::AddScript(std::shared_ptr<Script> sc)
{
	sc->SetScene(shared_from_this()); //Provide script reference to this scene
	scripts.push_back(sc);
}

void Scene::AddLight(std::shared_ptr<Light> light)
{
	lights.push_back(light);
}

void Scene::SetAmbientBrightness(float amb)
{
	lightManifest.ambientBrightness = amb;
}

std::shared_ptr<GameObject> Scene::FindObjectByID(Uint32 ID)
{
	for (int i = 0; i < gameObjects.size(); i++) 
	{
		if (gameObjects.at(i)->ID == ID) return gameObjects[i];
	}
	return nullptr;
}

std::shared_ptr<Light> Scene::FindLightByID(Uint32 ID)
{
	for (int i = 0; i < lights.size(); i++)
	{
		if (lights.at(i)->ID == ID) return lights[i];
	}
	return nullptr;
}

std::vector<std::shared_ptr<Light>> Scene::GetLights()
{
	return lights;
}

Scene::Scene(std::shared_ptr<Input> _input)
{
	input = _input;
}

Scene::~Scene()
{
	std::cout << "\n\nDESTROYING SCENE\n\n";
	std::cout << "GameObject count = " << gameObjects.size();
	std::cout << "\nScript Count = " << scripts.size() << "\n\n";
}
