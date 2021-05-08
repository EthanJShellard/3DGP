#pragma once
#include <memory>

class Scene;
class Input;

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