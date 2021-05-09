#include "GameObjectOBJ.h"
#include "glm/gtx/quaternion.hpp"

void GameObjectOBJ::SetModel(std::shared_ptr<OBJModel> newModel)
{
	model = newModel;
}

void GameObjectOBJ::Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, LightManifest lightManifest)
{
	UpdateModelMatrix();

	//Iterate through meshes and draw them with correct materials
	for (int i = 0; i < model->meshes.size(); i++)
	{
		//Bind the mesh VAO
		glBindVertexArray(model->meshes.at(i)->vao->GetID());
		//Apply the relevant material, binding the correct shader and passing in lighting info
		model->meshes.at(i)->material->Apply(modelMatrix, projection, invView, camPos, lightManifest);
		//Draw the mesh
		glDrawArrays(GL_TRIANGLES, 0, model->meshes.at(i)->vao->GetVertCount());
	}
}