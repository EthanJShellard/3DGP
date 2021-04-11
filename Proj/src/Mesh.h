#pragma once
#include "Material.h"
#include <memory>

/// <summary>
/// Class containing mesh information and the material relevant to rendering
/// </summary>
struct Mesh
{
	GLuint vao;
	int vertexCount;
	std::shared_ptr<Material> material;
	std::vector<GLuint> buffers;


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
		glDeleteBuffers(buffers.size(), &buffers.at(0));
	}
};