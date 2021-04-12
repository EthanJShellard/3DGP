#include "GameObjectOBJ.h"
#include "glm/gtx/quaternion.hpp"

void GameObjectOBJ::SetModel(std::shared_ptr<OBJModel> newModel)
{
	model = newModel;
}

void GameObjectOBJ::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3>lightPositions)
{
	//Update model matrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::toMat4(rotation) * modelMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

	//Iterate through meshes and draw them with correct materials
	for (int i = 0; i < model->meshes.size(); i++)
	{
		glBindVertexArray(model->meshes.at(i)->vao);
		model->meshes.at(i)->material->Apply(modelMatrix, projection, invView, camPos, lightPositions);
		glDrawArrays(GL_TRIANGLES, 0, model->meshes.at(i)->vertexCount);
	}
}