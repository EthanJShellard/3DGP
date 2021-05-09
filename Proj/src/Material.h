#pragma once
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "Shader.h"
#include "Light.h"
#include "Texture.h"
#include <memory>
#include <vector>

struct LightManifest;

//Contains information needed by shaders to properly display an object. Can be used to bind and apply material information to a shader.
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
	GLint emissiveColourLocation;
	//GLint specularColourLocation;
	GLint specularHighlightLocation;
	GLint dissolveLocation;
	GLint lightPositonsLocation; 
	GLint lightColoursLocation;
	GLint lightIntensitiesLocation;
	GLint lightCountLocation;
	GLint textureLocation;
	GLint camPositionLocation;
	GLint ambientBrightnessLocation;

	std::shared_ptr<Texture> texture;
	void SetShader(std::shared_ptr<Shader> newShader);
	//Bind shader and pass material and lighting information into uniforms
	void Apply(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 camPos, LightManifest lightManifest);

	Material();
	~Material();

private:
	std::shared_ptr<Shader> shader;
};