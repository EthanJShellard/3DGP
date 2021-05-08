#include "glm/ext.hpp"
#include "Material.h"
#include "LightManifest.h"
#include <exception>
#include <iostream>

void Material::SetShader(std::shared_ptr<Shader> newShader)
{
	shader = newShader;

	// Store location of uniforms and check if successfully found
	textureLocation = glGetUniformLocation(shader->GetID(), "u_Texture");
	modelMatLocation = glGetUniformLocation(shader->GetID(), "u_Model");
	projMatLocation = glGetUniformLocation(shader->GetID(), "u_Projection");
	viewMatLocation = glGetUniformLocation(shader->GetID(), "u_View");

	camPositionLocation = glGetUniformLocation(shader->GetID(), "u_camPos");
	lightPositonsLocation = glGetUniformLocation(shader->GetID(), "u_lightPositions");
	lightColoursLocation = glGetUniformLocation(shader->GetID(), "u_lightColors");
	lightIntensitiesLocation = glGetUniformLocation(shader->GetID(), "u_lightIntensities");
	lightCountLocation = glGetUniformLocation(shader->GetID(), "u_lightCount");
	dissolveLocation = glGetUniformLocation(shader->GetID(), "u_dissolve");
	specularHighlightLocation = glGetUniformLocation(shader->GetID(), "u_specularHighlight");
	emissiveColourLocation = glGetUniformLocation(shader->GetID(), "u_emissiveColour");

	if (
		textureLocation == -1 ||
		modelMatLocation == -1 ||
		projMatLocation == -1 ||
		viewMatLocation== -1 ||
		camPositionLocation == -1 ||
		lightPositonsLocation == -1 ||
		lightColoursLocation == -1 ||
		lightCountLocation == -1 ||
		lightIntensitiesLocation == -1 ||
		specularHighlightLocation == -1 ||
		emissiveColourLocation == -1
		) 
	{
		//throw std::exception();
	}

}


void Material::Apply(glm::mat4 model, glm::mat4 projection, glm::mat4 iView, glm::vec3 camPos, LightManifest manifest)
{
	//Bind shader program
	glUseProgram(shader->GetID());
	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	//Upload matrices
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(iView));

	glUniform3f(camPositionLocation, camPos.x, camPos.y, camPos.z);
	
	glUniform3fv(lightPositonsLocation, manifest.count, &manifest.lightPositions.at(0));
	glUniform3fv(lightColoursLocation, manifest.count, &manifest.lightColours.at(0));
	glUniform1fv(lightIntensitiesLocation, manifest.count, &manifest.lightIntensities.at(0));
	glUniform1i(lightCountLocation, manifest.count);

	glUniform1f(dissolveLocation, dissolve);
	glUniform1f(specularHighlightLocation, specularHighlights);
	glUniform3fv(emissiveColourLocation, 1, glm::value_ptr(emissiveColour));
}

Material::Material()
{
	ambientColour = glm::vec3(0);
	diffuseColour = glm::vec3(0);;
	specularColour = glm::vec3(0);;
	emissiveColour = glm::vec3(0);;
	specularHighlights = 999.0f;
	dissolve = 1.0f;
	opticalDensity = 1.0f;
	illuminationModel = 0;
}

Material::~Material()
{
	std::cout << "Destroying Material\n";
}





