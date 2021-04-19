#pragma once
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "Shader.h"
#include "Light.h"
#include <memory>
#include <vector>

struct Material
{
	//Material properties
	glm::vec3 ambientColour;
	glm::vec3 diffuseColour;
	glm::vec3 specularColour;
	glm::vec3 emissiveColour;
	float specularHighlights;
	float dissolve;
	float opticalDensity;
	int illuminationModel;

	//Vertex shader uniform locations
	GLint modelMatLocation;
	GLint invModelMatLocation;
	GLint viewMatLocation;
	GLint projMatLocation;

	//Frag shader uniform locations
	//GLint diffuseColourLocation;
	//GLint emissiveColourLocation;
	//GLint specularColourLocation;
	GLint specularHighlightLocation;
	GLint dissolveLocation;
	GLint lightPositonsLocation; 
	GLint lightColoursLocation;
	GLint lightCountLocation;
	GLint textureLocation;
	GLint camPositionLocation;

	GLuint texture;
	void SetShader(std::shared_ptr<Shader> newShader);
	void SetTextureFromFile(const char* path);
	void SetTexture(GLuint tex);
	void Apply(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 camPos, std::vector<float> lightPositions, std::vector<float> lightColours, int lightCount);

	Material();

	static unsigned char* LoadTextureData(const char* file, int* width, int* height);
	static GLuint CreateTexture(unsigned char* data, int width, int height);
private:
	std::shared_ptr<Shader> shader;
};