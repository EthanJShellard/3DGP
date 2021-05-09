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
	glm::vec3 m_ambientColour;
	glm::vec3 m_diffuseColour;
	glm::vec3 m_specularColour;
	glm::vec3 m_emissiveColour;
	float m_specularHighlights;
	float m_dissolve;
	float m_opticalDensity;
	int m_illuminationModel;

	//Vertex shader uniform locations
	GLint m_modelMatLocation;
	GLint m_invModelMatLocation;
	GLint m_viewMatLocation;
	GLint m_projMatLocation;

	//Frag shader uniform locations
	//GLint diffuseColourLocation;
	GLint m_emissiveColourLocation;
	//GLint specularColourLocation;
	GLint m_specularHighlightLocation;
	GLint m_dissolveLocation;
	GLint m_lightPositonsLocation; 
	GLint m_lightColoursLocation;
	GLint m_lightIntensitiesLocation;
	GLint m_lightCountLocation;
	GLint m_textureLocation;
	GLint m_camPositionLocation;
	GLint m_ambientBrightnessLocation;

	std::shared_ptr<Texture> m_texture;
	void SetShader(std::shared_ptr<Shader> m_newShader);
	//Bind shader and pass material and lighting information into uniforms
	void Apply(glm::mat4 _model, glm::mat4 _projection, glm::mat4 _view, glm::vec3 _camPos, LightManifest _lightManifest);

	Material();
	~Material();

private:
	std::shared_ptr<Shader> m_shader;
};