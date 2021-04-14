#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <vector>
#include <memory>

class Scene 
{
private:
	std::vector< std::shared_ptr<GameObject> > gameObjects;
	Camera mainCamera;

public:
	void Update(float deltaTime);
	void Draw();
};