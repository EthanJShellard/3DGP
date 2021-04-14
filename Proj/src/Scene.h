#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include <vector>
#include <memory>

class Scene 
{
private:
	std::vector< std::shared_ptr<GameObject> > gameObjects;
	std::vector< std::shared_ptr<Light> > lights;
	std::shared_ptr<Input> input;
	
public:
	Camera mainCamera;

	void Update(float deltaTime);
	void Draw(float windowWidth, float windowHeight);

	void AddObject(std::shared_ptr<GameObject> go);
	void AddLight(std::shared_ptr<Light> light);

	Scene(std::shared_ptr<Input> input);
};