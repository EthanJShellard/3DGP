#include "glm/ext.hpp"
#include "Material.h"
#include "LightManifest.h"
#include <exception>
#include <iostream>

void Material::SetShader(std::shared_ptr<Shader> _newShader)
{
	m_shader = _newShader;

	// Store location of available uniforms
	m_textureLocation = glGetUniformLocation(m_shader->GetID(), "u_Texture");
	m_modelMatLocation = glGetUniformLocation(m_shader->GetID(), "u_Model");
	m_projMatLocation = glGetUniformLocation(m_shader->GetID(), "u_Projection");
	m_viewMatLocation = glGetUniformLocation(m_shader->GetID(), "u_View");

	m_camPositionLocation = glGetUniformLocation(m_shader->GetID(), "u_camPos");
	m_lightPositonsLocation = glGetUniformLocation(m_shader->GetID(), "u_lightPositions");
	m_lightColoursLocation = glGetUniformLocation(m_shader->GetID(), "u_lightColors");
	m_lightIntensitiesLocation = glGetUniformLocation(m_shader->GetID(), "u_lightIntensities");
	m_lightCountLocation = glGetUniformLocation(m_shader->GetID(), "u_lightCount");
	m_dissolveLocation = glGetUniformLocation(m_shader->GetID(), "u_dissolve");
	m_specularHighlightLocation = glGetUniformLocation(m_shader->GetID(), "u_specularHighlight");
	m_emissiveColourLocation = glGetUniformLocation(m_shader->GetID(), "u_emissiveColour");
	m_ambientBrightnessLocation = glGetUniformLocation(m_shader->GetID(), "u_ambientBrightness");

}


void Material::Apply(glm::mat4 _model, glm::mat4 _projection, glm::mat4 _iView, glm::vec3 _camPos, LightManifest _manifest)
{
	//Bind shader program
	glUseProgram(m_shader->GetID());
	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->m_id);

	//Upload matrices
	glUniformMatrix4fv(m_modelMatLocation, 1, GL_FALSE, glm::value_ptr(_model));
	glUniformMatrix4fv(m_projMatLocation, 1, GL_FALSE, glm::value_ptr(_projection));
	glUniformMatrix4fv(m_viewMatLocation, 1, GL_FALSE, glm::value_ptr(_iView));

	//Upload camera position
	glUniform3f(m_camPositionLocation, _camPos.x, _camPos.y, _camPos.z);
	
	//Upload lighting information
	if (_manifest.m_count > 0) 
	{
		glUniform3fv(m_lightPositonsLocation, _manifest.m_count, &_manifest.m_lightPositions.at(0));
		glUniform3fv(m_lightColoursLocation, _manifest.m_count, &_manifest.m_lightColours.at(0));
		glUniform1fv(m_lightIntensitiesLocation, _manifest.m_count, &_manifest.m_lightIntensities.at(0));
	}
	glUniform1i(m_lightCountLocation, _manifest.m_count);
	glUniform1f(m_ambientBrightnessLocation, _manifest.m_ambientBrightness);
	
	//Upload material information
	glUniform1f(m_dissolveLocation, m_dissolve);
	glUniform1f(m_specularHighlightLocation, m_specularHighlights);
	glUniform3fv(m_emissiveColourLocation, 1, glm::value_ptr(m_emissiveColour));
}

Material::Material()
{
	m_ambientColour = glm::vec3(0);
	m_diffuseColour = glm::vec3(0);;
	m_specularColour = glm::vec3(0);;
	m_emissiveColour = glm::vec3(0);;
	m_specularHighlights = 999.0f;
	m_dissolve = 1.0f;
	m_opticalDensity = 1.0f;
	m_illuminationModel = 0;
}

Material::~Material()
{
	std::cout << "Destroying Material\n";
}





