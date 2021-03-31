#pragma once
#include "glm/glm.hpp"
#include "GL/glew.h"


struct Material
{
	glm::vec3 ambientColour;
	glm::vec3 diffuseColour;
	glm::vec3 specularColour;
	glm::vec3 emissiveColour;
	float specularHighlights;
	float dissolve;
	float opticalDensity;
	int illuminationModel;

	GLuint texture;

	void SetTextureFromFile(const char* path);

private:
	unsigned char* LoadTextureData(const char* file, int* width, int* height);
	GLuint CreateTexture(unsigned char* data, int width, int height);
};