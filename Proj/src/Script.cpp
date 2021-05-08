#include "Script.h"
#include "Input.h"
#include <iostream>

void Script::SetScene(std::shared_ptr<Scene> parentScene)
{
	scene = parentScene;
}

void Script::Update(float deltaTs, std::shared_ptr<Input> input)
{
}

void Script::Start()
{
}

Script::~Script()
{
	std::cout << "Destroying Script" << std::endl;
}
