#pragma once
#include "glm/gtx/quaternion.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "GameObject.h"
#include <memory>
#include <string>
#include <vector>

//Gameobject consisting of a single quad
class LoneQuad : public GameObject
{
private:
	std::shared_ptr<VertexArray> m_vao;
	std::vector< std::shared_ptr<VertexBuffer> > m_buffers;
	std::shared_ptr<Material> m_material;

	//Create an array object containing vertex, texture coordinate and normal information
	void BuildArrayObject();

public:
	LoneQuad(std::string _texturePath, std::shared_ptr<Shader> _shader);
	LoneQuad(std::shared_ptr<Texture> _tex, std::shared_ptr<Shader> _shader);

	void Draw(glm::mat4 _projection, glm::mat4 _invView, glm::vec3 _camPos, LightManifest _lightManifest) override;
};
