#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Material.h"

struct Object 
{
	GLuint vao;
	int vertexCount;
	std::shared_ptr<Material> material;

	Object()
	{
		vao = 0;
		material = nullptr;
		vertexCount = 0;
	}

	~Object() 
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteTextures(1, &(material->texture));
	}
};

struct Face 
{
	glm::vec3 pa;
	glm::vec3 pb;
	glm::vec3 pc;

	glm::vec2 tca;
	glm::vec2 tcb;
	glm::vec2 tcc;

	glm::vec3 na;
	glm::vec3 nb;
	glm::vec3 nc;

	glm::vec2 lmca;
	glm::vec2 lmcb;
	glm::vec2 lmcc;
};

/// <summary>
/// Class extending the provided bugl file to allow the loading objects with multiple textures
/// 
/// Some useful info can be found at http://paulbourke.net/dataformats/mtl/
/// </summary>
class OBJModel
{
private:

	void splitStringWhitespace(const std::string& input, std::vector<std::string>& output);
	void splitString(const std::string& input, char splitter, std::vector<std::string>& output);
	void loadModel(const std::string& objPath, std::string& currentLine);
	void loadModel(const std::string& path);
	void LoadMaterials(const std::string& path);

	std::unordered_map<std::string, std::shared_ptr<Material>> materials;
public:

	std::vector< std::shared_ptr<Object> > objects;
	

	OBJModel(std::string objPath);

};