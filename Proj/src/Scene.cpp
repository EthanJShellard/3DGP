#include "Scene.h"
#include <iostream>

void Scene::Start()
{
	//Initialise game objects
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Start();
	}
	//Initialise scripts
	for (int i = 0; i < m_scripts.size(); i++) 
	{
		m_scripts.at(i)->Start();
	}
}

void Scene::Update(float m_deltaTime)
{
	//Update camera
	m_mainCamera.Update(m_deltaTime, m_input);

	//Update game objects
	for (int i = 0; i < m_gameObjects.size(); i++) 
	{
		m_gameObjects.at(i)->Update(m_deltaTime, m_input);
	}

	//Update scripts
	for (int i = 0; i < m_scripts.size(); i++)
	{
		m_scripts.at(i)->Update(m_deltaTime, m_input);
	}

	//Update light manifest
	m_lightManifest.Update(m_lights);
}

void Scene::Draw(float _windowWidth, float _windowHeight)
{
	//Create perspective projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(m_mainCamera.m_vFOV),
		_windowWidth / _windowHeight, m_mainCamera.m_near, m_mainCamera.m_far);

	//Create view matrix
	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, m_mainCamera.m_transform.GetPosition()) * glm::toMat4(m_mainCamera.m_transform.GetQuaternionRotation()) * view;	

	//Draw each game object
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Draw(projection, glm::inverse(view), m_mainCamera.m_transform.GetPosition(), m_lightManifest);
	}
}

void Scene::AddObject(std::shared_ptr<GameObject> _go)
{
	_go->SetScene(shared_from_this()); //Provide object reference to this scene
	m_gameObjects.push_back(_go);
}

void Scene::AddScript(std::shared_ptr<Script> _sc)
{
	_sc->SetScene(shared_from_this()); //Provide script reference to this scene
	m_scripts.push_back(_sc);
}

void Scene::AddLight(std::shared_ptr<Light> _light)
{
	m_lights.push_back(_light);
}

void Scene::SetAmbientBrightness(float _ambientBrightness)
{
	m_lightManifest.m_ambientBrightness = _ambientBrightness;
}

std::shared_ptr<GameObject> Scene::FindObjectByID(Uint32 _ID)
{
	for (int i = 0; i < m_gameObjects.size(); i++) 
	{
		if (m_gameObjects.at(i)->m_ID == _ID) return m_gameObjects[i];
	}
	return nullptr;
}

std::shared_ptr<Light> Scene::FindLightByID(Uint32 _ID)
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		if (m_lights.at(i)->m_ID == _ID) return m_lights[i];
	}
	return nullptr;
}

std::vector<std::shared_ptr<Light>> Scene::GetLights()
{
	return m_lights;
}

Scene::Scene(std::shared_ptr<Input> _input)
{
	m_input = _input;
}

Scene::~Scene()
{
	std::cout << "\n\nDESTROYING SCENE\n\n";
	std::cout << "GameObject count = " << m_gameObjects.size();
	std::cout << "\nScript Count = " << m_scripts.size() << "\n\n";
}
