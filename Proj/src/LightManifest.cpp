#include "LightManifest.h"
#include "Light.h"


LightManifest::LightManifest()
{
	m_count = 0;
	m_ambientBrightness = 0.02; //Default ambient brightness
}

void LightManifest::Update(std::vector<std::shared_ptr<Light>> _lights)
{
	//Empty our vectors
	m_lightPositions.clear();
	m_lightColours.clear();
	m_lightIntensities.clear();

	//Make sure that we don't upload more lights than our hard limit. Magic number not ideal, but is fine for our purpose.
	m_count = std::min(_lights.size(), (size_t)10);

	if (m_count < 1)
	{
		m_lightPositions.push_back(0);
		m_lightPositions.push_back(0);
		m_lightPositions.push_back(0);

		m_lightColours.push_back(0);
		m_lightColours.push_back(0);
		m_lightColours.push_back(0);

		m_lightIntensities.push_back(0);
	}

	for (int i = 0; i < m_count; i++) 
	{
		//Position
		m_lightPositions.push_back(_lights.at(i)->m_transform.m_position.x);
		m_lightPositions.push_back(_lights.at(i)->m_transform.m_position.y);
		m_lightPositions.push_back(_lights.at(i)->m_transform.m_position.z);
		//Colour
		m_lightColours.push_back(_lights.at(i)->m_colour.x);
		m_lightColours.push_back(_lights.at(i)->m_colour.y);
		m_lightColours.push_back(_lights.at(i)->m_colour.z);
		//Intensity
		m_lightIntensities.push_back(_lights.at(i)->m_intensity);
	}
}