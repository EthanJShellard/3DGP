#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"

struct Light;

struct LightManifest
{
	std::vector<float> lightPositions;
	std::vector<float> lightColours;
	std::vector<float> lightIntensities;
	float ambientBrightness;
	int count;

	LightManifest();

	void Update(std::vector< std::shared_ptr<Light> > lights);
};