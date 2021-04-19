#include "GameObjectOBJ.h"
#include "glm/gtx/quaternion.hpp"

void GameObjectOBJ::SetModel(std::shared_ptr<OBJModel> newModel)
{
	model = newModel;
}

void GameObjectOBJ::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector< std::shared_ptr<Light> > lights)
{
	UpdateModelMatrix();

	std::vector<float> lightPositions;
	std::vector<float> lightColours;
	int count = std::min((int)lights.size(), 5);//We have a hard cap of 5 light sources

	for (int i = 0; i < count; i++) 
	{
		lightPositions.push_back(lights.at(i)->position.x);
		lightPositions.push_back(lights.at(i)->position.y);
		lightPositions.push_back(lights.at(i)->position.z);

		lightColours.push_back(lights.at(i)->colour.x);
		lightColours.push_back(lights.at(i)->colour.y);
		lightColours.push_back(lights.at(i)->colour.z);
	}

	//Iterate through meshes and draw them with correct materials
	for (int i = 0; i < model->meshes.size(); i++)
	{
		glBindVertexArray(model->meshes.at(i)->vao);
		model->meshes.at(i)->material->Apply(modelMatrix, projection, invView, camPos, lightPositions, lightColours, count);
		glDrawArrays(GL_TRIANGLES, 0, model->meshes.at(i)->vertexCount);
	}
}