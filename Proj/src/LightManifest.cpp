#include "LightManifest.h"
#include "Light.h"


LightManifest::LightManifest()
{
	count = 0;
	ambientBrightness = 0.02; //Default ambient brightness
}

void LightManifest::Update(std::vector<std::shared_ptr<Light>> lights)
{
	//Empty our vectors
	lightPositions.clear();
	lightColours.clear();
	lightIntensities.clear();

	//Make sure that we don't upload more lights than our hard limit. Magic number not ideal, but is fine for our purpose.
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
		//Position
		lightPositions.push_back(lights.at(i)->transform->position.x);
		lightPositions.push_back(lights.at(i)->transform->position.y);
		lightPositions.push_back(lights.at(i)->transform->position.z);
		//Colour
		lightColours.push_back(lights.at(i)->colour.x);
		lightColours.push_back(lights.at(i)->colour.y);
		lightColours.push_back(lights.at(i)->colour.z);
		//Intensity
		lightIntensities.push_back(lights.at(i)->intensity);
	}
}