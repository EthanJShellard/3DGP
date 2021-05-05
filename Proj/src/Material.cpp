#include "glm/ext.hpp"
#include "Material.h"
#include "LightManifest.h"
#include <stb_image.h>
#include <exception>

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
		specularHighlightLocation == -1
		) 
	{
		//throw std::exception();
	}

}

void Material::SetTextureFromFile(const char* path)
{
	if (texture) //If we're replacing the texture
	{
		glDeleteTextures(1, &texture);
	}

	int width;
	int height;

	//Load from file
	unsigned char* data = LoadTextureData(path, &width, &height);
	//Create texture in opengl
	texture = CreateTexture(data, width, height);
}

void Material::SetTexture(GLuint tex)
{
	texture = tex;
}

void Material::Apply(glm::mat4 model, glm::mat4 projection, glm::mat4 iView, glm::vec3 camPos, LightManifest manifest)
{
	//Bind shader program
	glUseProgram(shader->GetID());
	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

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
}

Material::Material()
{
	ambientColour = glm::vec3(1);
	diffuseColour = glm::vec3(1);;
	specularColour = glm::vec3(1);;
	emissiveColour = glm::vec3(1);;
	specularHighlights = 999.0f;
	dissolve = 1.0f;
	opticalDensity = 1.0f;
	illuminationModel = 0;
}

unsigned char* Material::LoadTextureData(const char* file, int* width, int* height)
{
	unsigned char* data = stbi_load(file, width, height, NULL, 4);

	if (!data) throw std::exception();

	return data;
}

/// <summary>
/// Generates a texture in the context of opengl
/// </summary>
GLuint Material::CreateTexture(unsigned char* data, int width, int height)
{
	//Create and bind texture
	GLuint texID = 0;
	glGenTextures(1, &texID);

	if (!texID)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, texID);

	//Upload image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//Generate MipMap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

