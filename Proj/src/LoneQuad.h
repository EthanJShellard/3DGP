#pragma once
#include "glm/gtx/quaternion.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "GameObject.h"
#include <memory>
#include <string>
#include <vector>

class LoneQuad : public GameObject
{
private:
	std::shared_ptr<VertexArray> vao;
	std::vector< std::shared_ptr<VertexBuffer> > buffers;
	std::shared_ptr<Material> material;

	void BuildArrayObject();

public:
	LoneQuad(std::string texturePath, std::shared_ptr<Shader> shader);
	LoneQuad(GLuint texture, std::shared_ptr<Shader> shader);

	void Draw(glm::mat4 projection, glm::mat4 invView, glm::vec3 camPos, std::vector<glm::vec3>lightPositions) override;
};
