#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include <vector>
#include <memory>

class GameObject;

class Scene : public std::enable_shared_from_this<Scene>
{
private:
	std::vector< std::shared_ptr<GameObject> > gameObjects;
	std::vector< std::shared_ptr<Light> > lights;
	std::shared_ptr<Input> input;

public:
	Camera mainCamera;

	void Start();
	void Update(float deltaTime);
	void Draw(float windowWidth, float windowHeight);

	void AddObject(std::shared_ptr<GameObject> go);
	void AddLight(std::shared_ptr<Light> light);
	std::shared_ptr<GameObject> FindObjectByID(Uint32 ID);
	std::shared_ptr<Light> FindLightByID(Uint32 ID);
	std::vector< std::shared_ptr<Light> > GetLights();

	Scene(std::shared_ptr<Input> input);
};