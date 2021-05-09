#pragma once
#include <memory>

class Scene;
class Input;

//Parent class for scripts which are to be run in a scene.
class Script 
{
protected:
	std::weak_ptr<Scene> m_scene;

public:
	void SetScene(std::shared_ptr<Scene> _parentScene);
	virtual void Update(float _deltaTs, std::shared_ptr<Input> _input);
	virtual void Start();

	~Script();
};