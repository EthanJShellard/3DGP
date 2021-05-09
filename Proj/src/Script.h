#pragma once
#include <memory>

class Scene;
class Input;

//Parent class for scripts which are to be run in a scene.
class Script 
{
protected:
	std::weak_ptr<Scene> scene;

public:
	void SetScene(std::shared_ptr<Scene> parentScene);
	virtual void Update(float deltaTs, std::shared_ptr<Input> input);
	virtual void Start();

	~Script();
};