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
/// Class extending the provided bugl file to allow the loading objects with multiple textures.
/// 
/// Some useful info can be found at http://paulbourke.net/dataformats/mtl/
/// </summary>
class OBJModel
{
private:

	void splitStringWhitespace(const std::string& _input, std::vector<std::string>& _output);
	void splitString(const std::string& _input, char _splitter, std::vector<std::string>& _output);
	void loadModel(const std::string& _objPath, std::string& _currentLine);
	void loadModel(const std::string& _path);
	void LoadMaterials(const std::string& _path, std::string& _currentLine, std::unordered_map<std::string, std::shared_ptr<Material> >& _map);

public:

	std::vector< std::shared_ptr<Mesh> > m_meshes;
	

	OBJModel(std::string _objPath, std::shared_ptr<Shader> _shader);

};