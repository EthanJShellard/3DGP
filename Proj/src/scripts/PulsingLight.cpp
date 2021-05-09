#include "PulsingLight.h"
#include "../Scene.h"


void PulsingLight::Update(float _deltaTs, std::shared_ptr<Input> _input)
{
	if (auto lPtr = m_light.lock()) 
	{
		if (m_rising) 
		{
			lPtr->m_intensity += m_speed * _deltaTs;
			if (lPtr->m_intensity >= m_maxIntensity) m_rising = false;
		}
		else 
		{
			lPtr->m_intensity -= m_speed * _deltaTs;
			if (lPtr->m_intensity <= m_minIntensity) m_rising = true;
		}
	}
}

void PulsingLight::Start()
{
	if (auto scPtr = m_scene.lock()) 
	{
		std::shared_ptr<Light> lightPtr = std::make_shared<Light>(m_position, m_colour, m_minIntensity);
		scPtr->AddLight(lightPtr);
		m_light = lightPtr;
	}
}

PulsingLight::PulsingLight(float _minIntensity, float _maxIntensity, float _speed, glm::vec3 _position, glm::vec3 _colour)
{
	m_minIntensity = _minIntensity;
	m_maxIntensity = _maxIntensity;
	m_speed = _speed;
	m_position = _position;
	m_colour = _colour;
	m_rising = true;
}