#include "Script.h"
#include "Input.h"
#include <iostream>

void Script::SetScene(std::shared_ptr<Scene> _parentScene)
{
	m_scene = _parentScene;
}

void Script::Update(float _deltaTs, std::shared_ptr<Input> _input)
{
}

void Script::Start()
{
}

Script::~Script()
{
	std::cout << "Destroying Script" << std::endl;
}
