#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Material.h"
#include "Mesh.h"

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
	void LoadMaterials(const std::string& path, std::string& currentLine);

	std::unordered_map<std::string, std::shared_ptr<Material>> materials;
public:

	std::vector< std::shared_ptr<Mesh> > meshes;
	

	OBJModel(std::string objPath, std::shared_ptr<Shader> shader);

};