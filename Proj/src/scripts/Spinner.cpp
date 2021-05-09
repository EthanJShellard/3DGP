#include "Spinner.h"

void Spinner::Update(float _deltaTs, std::shared_ptr<Input> _input)
{
	//Rotate target object
	if (auto targetPtr = m_targetObject.lock()) 
	{
		targetPtr->Rotate(m_speed * _deltaTs, m_axis);
	}

}

void Spinner::Start()
{
	//Get target object
	if (auto scPtr = m_scene.lock()) 
	{
		m_targetObject = scPtr->FindObjectByID(m_targetID);
	}
}

Spinner::Spinner(float _speed, glm::vec3 _axis, int _targetID)
{
	m_speed = _speed;
	m_axis = _axis;
	m_targetID = _targetID;
}