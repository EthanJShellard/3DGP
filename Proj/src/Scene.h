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
	std::vector< std::shared_ptr<GameObject> > gameObjects;
	std::vector< std::shared_ptr<Script> > scripts;
	std::vector< std::shared_ptr<Light> > lights;
	std::shared_ptr<Input> input;

	LightManifest lightManifest;

public:
	Camera mainCamera;

	void Start();
	void Update(float deltaTime);
	void Draw(float windowWidth, float windowHeight);

	void AddObject(std::shared_ptr<GameObject> go);
	void AddScript(std::shared_ptr<Script> sc);
	void AddLight(std::shared_ptr<Light> light);
	void SetAmbientBrightness(float amb);
	std::shared_ptr<GameObject> FindObjectByID(Uint32 ID);
	std::shared_ptr<Light> FindLightByID(Uint32 ID);
	std::vector< std::shared_ptr<Light> > GetLights();

	Scene(std::shared_ptr<Input> input);
	~Scene();
};