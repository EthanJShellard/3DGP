#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "LightManifest.h"
#include "Script.h"
#include <vector>
#include <memory>

class GameObject;

//Contains collections of objects and scripts necesary to run a scene. Will populate a light manifest and call start, update and draw functions on compatible objects.
class Scene : public std::enable_shared_from_this<Scene>
{
private:
	std::vector< std::shared_ptr<GameObject> > m_gameObjects;
	std::vector< std::shared_ptr<Script> > m_scripts;
	std::vector< std::shared_ptr<Light> > m_lights;
	std::shared_ptr<Input> m_input;

	LightManifest m_lightManifest;

public:
	Camera m_mainCamera;

	void Start();
	void Update(float _deltaTime);
	void Draw(float _windowWidth, float _windowHeight);

	void AddObject(std::shared_ptr<GameObject> _go);
	void AddScript(std::shared_ptr<Script> _sc);
	void AddLight(std::shared_ptr<Light> _light);
	void SetAmbientBrightness(float _ambientBrightness);
	std::shared_ptr<GameObject> FindObjectByID(Uint32 _ID);
	std::shared_ptr<Light> FindLightByID(Uint32 _ID);
	std::vector< std::shared_ptr<Light> > GetLights();

	Scene(std::shared_ptr<Input> _input);
	~Scene();
};