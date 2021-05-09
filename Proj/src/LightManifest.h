#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"

struct Light;

//Manifest containing lighting information needed by shaders
struct LightManifest
{
	std::vector<float> m_lightPositions;
	std::vector<float> m_lightColours;
	std::vector<float> m_lightIntensities;
	float m_ambientBrightness;
	int m_count;

	LightManifest();

	//Discard old information and fill the manifest with updated information
	void Update(std::vector< std::shared_ptr<Light> > _lights);
};