#pragma once

#include <vector>;
#include <memory>

struct Light;

struct LightManifest
{
	std::vector<float> lightPositions;
	std::vector<float> lightColours;
	std::vector<float> lightIntensities;

	int count;

	LightManifest();

	void Update(std::vector< std::shared_ptr<Light> > lights);
};