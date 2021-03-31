#pragma once
#include "Material.h"
#include <memory>


struct Mesh
{
	GLuint vao;
	int vertexCount;
	std::shared_ptr<Material> material;

	Mesh()
	{
		vao = 0;
		material = nullptr;
		vertexCount = 0;
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteTextures(1, &(material->texture));
	}
};