#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"

struct Light;

//Manifest containing lighting information needed by shaders
struct LightManifest
{
	std::vector<float> lightPositions;
	std::vector<float> lightColours;
	std::vector<float> lightIntensities;
	float ambientBrightness;
	int count;

	LightManifest();

	//Discard old information and fill the manifest with updated information
	void Update(std::vector< std::shared_ptr<Light> > lights);
};