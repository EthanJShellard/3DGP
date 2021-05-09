#include "OBJModel.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <iostream>
#include <fstream>

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

void OBJModel::splitStringWhitespace(const std::string& _input, std::vector<std::string>& _output)
{
	std::string curr;

	_output.clear();

	for (size_t i = 0; i < _input.length(); i++)
	{
		if (_input.at(i) == ' ' ||
			_input.at(i) == '\r' ||
			_input.at(i) == '\n' ||
			_input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				_output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += _input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		_output.push_back(curr);
	}
}

void OBJModel::splitString(const std::string& _input, char _splitter, std::vector<std::string>& _output)
{
	std::string curr;

	_output.clear();

	for (size_t i = 0; i < _input.length(); i++)
	{
		if (_input.at(i) == _splitter)
		{
			_output.push_back(curr);
			curr = "";
		}
		else
		{
			curr += _input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		_output.push_back(curr);
	}
}

void OBJModel::LoadMaterials(const std::string& _path, std::string &_currentLine, std::unordered_map<std::string, std::shared_ptr<Material> >& _map)
{
	std::ifstream file(_path.c_str());
	std::shared_ptr<Material> currentMaterial;

	if (!file.is_open()) 
	{
		_currentLine.append(" (Failed to open file)");
		throw std::exception();
	}

	while(!file.eof()) 
	{
		std::getline(file, _currentLine);
		if (_currentLine.length() < 1) continue; //If line is empty go to next line

		std::vector<std::string> tokens;
		splitStringWhitespace(_currentLine, tokens);
		if (tokens.size() < 1) continue; //If no tokens returned go to next line

		if (tokens.at(0) == "newmtl") //New material
		{
			currentMaterial = std::make_shared<Material>();
			_map.insert(std::make_pair(tokens.at(1), currentMaterial));
		}
		else if (tokens.at(0) == "Ns") //Specular highlights define
		{
			currentMaterial->m_specularHighlights = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "Ka") //Ambient colour define
		{
			currentMaterial->m_ambientColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Kd") //Diffuse colour define
		{
			currentMaterial->m_diffuseColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Ks") //Specular colour define
		{
			currentMaterial->m_specularColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Ke") //Emissive colour define
		{
			currentMaterial->m_emissiveColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Ni") //Optical density define
		{
			currentMaterial->m_opticalDensity = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "d") //Dissolve define
		{
			currentMaterial->m_dissolve = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "illum") //Illumination model define
		{
			currentMaterial->m_illuminationModel = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "map_Kd") //Texture define - does not account for spaces in material file names!!!
		{
			std::string newPath = _path.substr(0, _path.find_last_of('/') + 1);
			newPath.append(tokens.at(1));

			currentMaterial->m_texture = std::make_shared<Texture>(newPath.c_str());
		}
	}

	if (!currentMaterial->m_texture) //If no texture was defined, provide default texture
	{
		currentMaterial->m_texture = std::make_shared<Texture>("assets/models/Demo/WhiteSquare.png");
	}
}

void OBJModel::loadModel(const std::string& _objPath, std::string& _currentLine)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> tcs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> lmcs;
	std::vector<Face> faces;

	bool materialsLoaded = false;
	std::shared_ptr<Material> currentMaterial;
	std::unordered_map<std::string, std::shared_ptr<Material> > materials;

	int counter = 0;

	std::ifstream file(_objPath.c_str());

	if (!file.is_open())
	{
		throw std::exception();
	}

	while (!file.eof())
	{
		std::getline(file, _currentLine);
		if (_currentLine.length() < 1) continue;
		//std::cout << "Line [" << currentLine << "]" << std::endl;

		std::vector<std::string> tokens;
		splitStringWhitespace(_currentLine, tokens);
		if (tokens.size() < 1) continue;

		if (tokens.at(0) == "v") //Vertex definition
		{
			glm::vec3 p(atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str()));
			positions.push_back(p);
		}
		else if (tokens.at(0) == "vt") //Texture coordinate definition
		{
			glm::vec2 tc(atof(tokens.at(1).c_str()),
				1.0f - atof(tokens.at(2).c_str()));

			tcs.push_back(tc);
		}
		else if (tokens.at(0) == "vn") //Vertex normal definition
		{
			glm::vec3 n(atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str()));

			normals.push_back(n);
		}
		else if (tokens.at(0) == "f") //Face definition
		{
			if (tokens.size() < 4) continue;

			Face f;
			std::vector<std::string> sub;
			splitString(tokens.at(1), '/', sub);
			if (sub.size() >= 1) f.pa = positions.at(atoi(sub.at(0).c_str()) - 1);
			if (sub.size() >= 2) f.tca = tcs.at(atoi(sub.at(1).c_str()) - 1);
			if (sub.size() >= 3) f.na = normals.at(atoi(sub.at(2).c_str()) - 1);
			if (sub.size() >= 4) f.lmca = lmcs.at(atoi(sub.at(3).c_str()) - 1);

			splitString(tokens.at(2), '/', sub);
			if (sub.size() >= 1) f.pb = positions.at(atoi(sub.at(0).c_str()) - 1);
			if (sub.size() >= 2) f.tcb = tcs.at(atoi(sub.at(1).c_str()) - 1);
			if (sub.size() >= 3) f.nb = normals.at(atoi(sub.at(2).c_str()) - 1);
			if (sub.size() >= 4) f.lmcb = lmcs.at(atoi(sub.at(3).c_str()) - 1);

			splitString(tokens.at(3), '/', sub);
			if (sub.size() >= 1) f.pc = positions.at(atoi(sub.at(0).c_str()) - 1);
			if (sub.size() >= 2) f.tcc = tcs.at(atoi(sub.at(1).c_str()) - 1);
			if (sub.size() >= 3) f.nc = normals.at(atoi(sub.at(2).c_str()) - 1);
			if (sub.size() >= 4) f.lmcc = lmcs.at(atoi(sub.at(3).c_str()) - 1);

			faces.push_back(f);
			if (tokens.size() < 5) continue;
			Face fq;
			fq.pa = f.pc;
			fq.tca = f.tcc;
			fq.na = f.nc;
			fq.lmca = f.lmcc;

			splitString(tokens.at(4), '/', sub);
			if (sub.size() >= 1) fq.pb = positions.at(atoi(sub.at(0).c_str()) - 1);
			if (sub.size() >= 2) fq.tcb = tcs.at(atoi(sub.at(1).c_str()) - 1);
			if (sub.size() >= 3) fq.nb = normals.at(atoi(sub.at(2).c_str()) - 1);
			if (sub.size() >= 4) fq.lmcb = lmcs.at(atoi(sub.at(3).c_str()) - 1);

			fq.pc = f.pa;
			fq.tcc = f.tca;
			fq.nc = f.na;
			fq.lmcc = f.lmca;

			faces.push_back(fq);
		}
		else if (tokens.at(0) == "mtllib") //Load material library
		{
			std::string newPath = _objPath.substr(0, _objPath.find_last_of('/') + 1);
			newPath.append(tokens.at(1));
			LoadMaterials(newPath, _currentLine, materials);
			materialsLoaded = true;
		}
		else if (tokens.at(0) == "usemtl") //Start of material group
		{
			if (currentMaterial) //If this marks the end of one material group/ object 
			{
				std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
				mesh->m_material = currentMaterial;

				mesh->m_vao = std::make_shared<VertexArray>();


				if (faces.size() > 0) 
				{
					mesh->m_vao->SetVertCount(faces.size() * 3); //3 vertices for each triangulated face

					std::shared_ptr<VertexBuffer> b = std::make_shared <VertexBuffer>();

					for (std::vector<Face>::iterator fit = faces.begin();
						fit != faces.end(); fit++)
					{
						b->Add(fit->pa.x, fit->pa.y, fit->pa.z);
						b->Add(fit->pb.x, fit->pb.y, fit->pb.z);
						b->Add(fit->pc.x, fit->pc.y, fit->pc.z);
					}

					mesh->m_vao->SetBuffer(b, 0);

				} //End if faces > 0

				if (tcs.size() > 0)
				{
					std::shared_ptr<VertexBuffer> b = std::make_shared <VertexBuffer>();

					for (std::vector<Face>::iterator fit = faces.begin();
						fit != faces.end(); fit++)
					{
						b->Add(fit->tca.x, fit->tca.y);
						b->Add(fit->tcb.x, fit->tcb.y);
						b->Add(fit->tcc.x, fit->tcc.y);
					}

					mesh->m_vao->SetBuffer(b, 1);
				}

				if (normals.size() > 0)
				{
					std::shared_ptr<VertexBuffer> b = std::make_shared <VertexBuffer>();

					for (std::vector<Face>::iterator fit = faces.begin();
						fit != faces.end(); fit++)
					{
						b->Add(fit->na.x, fit->na.y, fit->na.z);
						b->Add(fit->nb.x, fit->nb.y, fit->nb.z);
						b->Add(fit->nc.x, fit->nc.y, fit->nc.z);
					}

					mesh->m_vao->SetBuffer(b, 2);
				}
				
				m_meshes.push_back(mesh);

			}//End creation of object (if(currentMaterial))

			currentMaterial = materials.find(tokens.at(1))->second;
			faces.clear();
		}
	}
	//Collect final object
	if (currentMaterial) //If this marks the end of one material group/ object 
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->m_material = currentMaterial;

		mesh->m_vao = std::make_shared<VertexArray>();


		if (faces.size() > 0)
		{
			mesh->m_vao->SetVertCount(faces.size() * 3); //3 vertices for each triangulated face

			std::shared_ptr<VertexBuffer> b = std::make_shared <VertexBuffer>();

			for (std::vector<Face>::iterator fit = faces.begin();
				fit != faces.end(); fit++)
			{
				b->Add(fit->pa.x, fit->pa.y, fit->pa.z);
				b->Add(fit->pb.x, fit->pb.y, fit->pb.z);
				b->Add(fit->pc.x, fit->pc.y, fit->pc.z);
			}

			mesh->m_vao->SetBuffer(b, 0);

		} //End if faces > 0

		if (tcs.size() > 0)
		{
			std::shared_ptr<VertexBuffer> b = std::make_shared <VertexBuffer>();

			for (std::vector<Face>::iterator fit = faces.begin();
				fit != faces.end(); fit++)
			{
				b->Add(fit->tca.x, fit->tca.y);
				b->Add(fit->tcb.x, fit->tcb.y);
				b->Add(fit->tcc.x, fit->tcc.y);
			}

			mesh->m_vao->SetBuffer(b, 1);
		}

		if (normals.size() > 0)
		{
			std::shared_ptr<VertexBuffer> b = std::make_shared <VertexBuffer>();

			for (std::vector<Face>::iterator fit = faces.begin();
				fit != faces.end(); fit++)
			{
				b->Add(fit->na.x, fit->na.y, fit->na.z);
				b->Add(fit->nb.x, fit->nb.y, fit->nb.z);
				b->Add(fit->nc.x, fit->nc.y, fit->nc.z);
			}

			mesh->m_vao->SetBuffer(b, 2);
		}

		m_meshes.push_back(mesh);

	}//End creation of object (if(currentMaterial))

}

void OBJModel::loadModel(const std::string& _path)
{
	std::string currentLine = _path;

	try
	{
		return loadModel(_path, currentLine);
	}
	catch (std::exception& e)
	{
		std::cout << "Failed to parse model data [" + currentLine + "]"
			<< std::endl;

		throw std::exception();
		//throw Exception("Failed to parse model data [" + currentLine + "]");
	}
}

OBJModel::OBJModel(std::string _objPath, std::shared_ptr<Shader> _defaultShader)
{
	loadModel(_objPath);

	for (int i = 0; i < m_meshes.size(); i++) 
	{
		m_meshes.at(i)->m_material->SetShader(_defaultShader);
	}
}

