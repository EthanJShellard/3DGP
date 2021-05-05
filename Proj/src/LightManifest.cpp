#include "LightManifest.h"
#include "Light.h"


LightManifest::LightManifest()
{
	count = 0;
}

void LightManifest::Update(std::vector<std::shared_ptr<Light>> lights)
{
	lightPositions.clear();
	lightColours.clear();
	lightIntensities.clear();

	count = std::min(lights.size(), (size_t)10);

	if (count < 1)
	{
		lightPositions.push_back(0);
		lightPositions.push_back(0);
		lightPositions.push_back(0);

		lightColours.push_back(0);
		lightColours.push_back(0);
		lightColours.push_back(0);

		lightIntensities.push_back(0);
	}

	for (int i = 0; i < count; i++) 
	{
		lightPositions.push_back(lights.at(i)->transform->position.x);
		lightPositions.push_back(lights.at(i)->transform->position.y);
		lightPositions.push_back(lights.at(i)->transform->position.z);

		lightColours.push_back(lights.at(i)->colour.x);
		lightColours.push_back(lights.at(i)->colour.y);
		lightColours.push_back(lights.at(i)->colour.z);

		lightIntensities.push_back(lights.at(i)->intensity);
	}
}