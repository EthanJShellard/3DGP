#include "GameObjectOBJ.h"
#include "glm/gtx/quaternion.hpp"

void GameObjectOBJ::SetModel(std::shared_ptr<OBJModel> _newModel)
{
	m_model = _newModel;
}

void GameObjectOBJ::Draw(glm::mat4 _projection, glm::mat4 _invView, glm::vec3 _camPos, LightManifest _lightManifest)
{
	UpdateModelMatrix();

	//Iterate through meshes and draw them with correct materials
	for (int i = 0; i < m_model->m_meshes.size(); i++)
	{
		//Bind the mesh VAO
		glBindVertexArray(m_model->m_meshes.at(i)->m_vao->GetID());
		//Apply the relevant material, binding the correct shader and passing in lighting info
		m_model->m_meshes.at(i)->m_material->Apply(m_modelMatrix, _projection, _invView, _camPos, _lightManifest);
		//Draw the mesh
		glDrawArrays(GL_TRIANGLES, 0, m_model->m_meshes.at(i)->m_vao->GetVertCount());
	}
}