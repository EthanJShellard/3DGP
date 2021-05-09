#include "Spinner.h"

void Spinner::Update(float deltaTs, std::shared_ptr<Input> input)
{
	//Rotate target object
	if (auto targetPtr = targetObject.lock()) 
	{
		targetPtr->Rotate(speed * deltaTs, axis);
	}

}

void Spinner::Start()
{
	//Get target object
	if (auto scPtr = scene.lock()) 
	{
		targetObject = scPtr->FindObjectByID(targetID);
	}
}

Spinner::Spinner(float _speed, glm::vec3 _axis, int _targetID)
{
	speed = _speed;
	axis = _axis;
	targetID = _targetID;
}