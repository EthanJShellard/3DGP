#include "OBJModel.h"
#include "Material.h"
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

void OBJModel::splitStringWhitespace(const std::string& input, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ' ||
			input.at(i) == '\r' ||
			input.at(i) == '\n' ||
			input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

void OBJModel::splitString(const std::string& input, char splitter, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == splitter)
		{
			output.push_back(curr);
			curr = "";
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

void OBJModel::LoadMaterials(const std::string& path, std::string &currentLine) 
{
	std::ifstream file(path.c_str());
	std::shared_ptr<Material> currentMaterial;


	if (!file.is_open()) 
	{
		currentLine.append(" (Failed to open file)");
		throw std::exception();
	}

	while(!file.eof()) 
	{
		std::getline(file, currentLine);
		if (currentLine.length() < 1) continue; //If line is empty go to next line

		std::vector<std::string> tokens;
		splitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue; //If no tokens returned go to next line

		if (tokens.at(0) == "newmtl") 
		{
			currentMaterial = std::make_shared<Material>();
			materials.insert(std::make_pair(tokens.at(1), currentMaterial));
		}
		else if (tokens.at(0) == "Ns") 
		{
			currentMaterial->specularHighlights = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "Ka") 
		{
			currentMaterial->ambientColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Kd") 
		{
			currentMaterial->diffuseColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Ks") 
		{
			currentMaterial->specularColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Ke") 
		{
			currentMaterial->emissiveColour = glm::vec3
			(
				atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str())
			);
		}
		else if (tokens.at(0) == "Ni") 
		{
			currentMaterial->opticalDensity = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "d") 
		{
			currentMaterial->dissolve = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "illum") 
		{
			currentMaterial->illuminationModel = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "map_Kd") //Does not account for spaces in material file names!!!
		{
			std::string newPath = path.substr(0, path.find_last_of('/') + 1);
			newPath.append(tokens.at(1));

			currentMaterial->SetTextureFromFile(newPath.c_str());
		}
	}
}

void OBJModel::loadModel(const std::string& objPath, std::string& currentLine)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> tcs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> lmcs;
	std::vector<Face> faces;

	bool materialsLoaded = false;
	std::shared_ptr<Material> currentMaterial;

	int counter = 0;

	std::ifstream file(objPath.c_str());

	if (!file.is_open())
	{
		throw std::exception();
	}

	while (!file.eof())
	{
		std::getline(file, currentLine);
		if (currentLine.length() < 1) continue;
		//std::cout << "Line [" << currentLine << "]" << std::endl;

		std::vector<std::string> tokens;
		splitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue;

		if (tokens.at(0) == "v")
		{
			glm::vec3 p(atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str()));
			positions.push_back(p);
		}
		else if (tokens.at(0) == "vt")
		{
			glm::vec2 tc(atof(tokens.at(1).c_str()),
				1.0f - atof(tokens.at(2).c_str()));

			tcs.push_back(tc);
		}
		else if (tokens.at(0) == "vn")
		{
			glm::vec3 n(atof(tokens.at(1).c_str()),
				atof(tokens.at(2).c_str()),
				atof(tokens.at(3).c_str()));

			normals.push_back(n);
		}
		else if (tokens.at(0) == "f")
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
		else if (tokens.at(0) == "mtllib") 
		{
			std::string newPath = objPath.substr(0, objPath.find_last_of('/') + 1);
			newPath.append(tokens.at(1));
			LoadMaterials(newPath, currentLine);
			materialsLoaded = true;
		}
		else if (tokens.at(0) == "usemtl")
		{
			if (currentMaterial) //If this marks the end of one material group/ object 
			{
				std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
				mesh->material = currentMaterial;

				glGenVertexArrays(1, &mesh->vao);

				if (!mesh->vao)
				{
					throw std::exception();
				}

				GLuint vboId;

				if (faces.size() > 0) 
				{
					mesh->vertexCount = faces.size() * 3; //3 vertices for each triangulated face

					std::vector<float> b;
					for (std::vector<Face>::iterator fit = faces.begin();
						fit != faces.end(); fit++)
					{
						b.push_back(fit->pa.x); b.push_back(fit->pa.y); b.push_back(fit->pa.z);
						b.push_back(fit->pb.x); b.push_back(fit->pb.y); b.push_back(fit->pb.z);
						b.push_back(fit->pc.x); b.push_back(fit->pc.y); b.push_back(fit->pc.z);
					}
					glGenBuffers(1, &vboId);
					mesh->buffers.push_back(vboId);

					if (!vboId)
					{
						throw std::exception();
					}

					glBindBuffer(GL_ARRAY_BUFFER, vboId);

					glBufferData(GL_ARRAY_BUFFER, sizeof(b.at(0)) * b.size(), &b.at(0),
						GL_STATIC_DRAW);

					glBindVertexArray(mesh->vao);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
					glEnableVertexAttribArray(0);
					glBindVertexArray(0);

				} //End if faces > 0

				if (tcs.size() > 0)
				{
					std::vector<float> b;

					for (std::vector<Face>::iterator fit = faces.begin();
						fit != faces.end(); fit++)
					{
						b.push_back(fit->tca.x); b.push_back(fit->tca.y);
						b.push_back(fit->tcb.x); b.push_back(fit->tcb.y);
						b.push_back(fit->tcc.x); b.push_back(fit->tcc.y);
					}

					glGenBuffers(1, &vboId);
					mesh->buffers.push_back(vboId);

					if (!vboId)
					{
						throw std::exception();
					}

					glBindBuffer(GL_ARRAY_BUFFER, vboId);

					glBufferData(GL_ARRAY_BUFFER, sizeof(b.at(0)) * b.size(), &b.at(0),
						GL_STATIC_DRAW);

					glBindVertexArray(mesh->vao);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
					glEnableVertexAttribArray(1);
					glBindVertexArray(0);
				}

				if (normals.size() > 0)
				{
					std::vector<float> b;

					for (std::vector<Face>::iterator fit = faces.begin();
						fit != faces.end(); fit++)
					{
						b.push_back(fit->na.x); b.push_back(fit->na.y); b.push_back(fit->na.z);
						b.push_back(fit->nb.x); b.push_back(fit->nb.y); b.push_back(fit->nb.z);
						b.push_back(fit->nc.x); b.push_back(fit->nc.y); b.push_back(fit->nc.z);
					}

					glGenBuffers(1, &vboId);
					mesh->buffers.push_back(vboId);

					if (!vboId)
					{
						throw std::exception();
					}

					glBindBuffer(GL_ARRAY_BUFFER, vboId);

					glBufferData(GL_ARRAY_BUFFER, sizeof(b.at(0)) * b.size(), &b.at(0),
						GL_STATIC_DRAW);

					glBindVertexArray(mesh->vao);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
					glEnableVertexAttribArray(2);
					glBindVertexArray(0);
				}
				
				meshes.push_back(mesh);

			}//End creation of object (if(currentMaterial))

			currentMaterial = materials.find(tokens.at(1))->second;
			faces.clear();
		}
	}
	//Collect final object
	if (currentMaterial) //If this marks the end of one material group/ object 
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->material = currentMaterial;

		glGenVertexArrays(1, &mesh->vao);

		if (!mesh->vao)
		{
			throw std::exception();
		}

		GLuint vboId;

		if (faces.size() > 0)
		{
			mesh->vertexCount += faces.size() * 3; //3 vertices for each triangulated face

			std::vector<float> b;

			for (std::vector<Face>::iterator fit = faces.begin();
				fit != faces.end(); fit++)
			{
				b.push_back(fit->pa.x); b.push_back(fit->pa.y); b.push_back(fit->pa.z);
				b.push_back(fit->pb.x); b.push_back(fit->pb.y); b.push_back(fit->pb.z);
				b.push_back(fit->pc.x); b.push_back(fit->pc.y); b.push_back(fit->pc.z);
			}

			glGenBuffers(1, &vboId);
			mesh->buffers.push_back(vboId);

			if (!vboId)
			{
				throw std::exception();
			}

			glBindBuffer(GL_ARRAY_BUFFER, vboId);

			glBufferData(GL_ARRAY_BUFFER, sizeof(b.at(0)) * b.size(), &b.at(0),
				GL_STATIC_DRAW);

			glBindVertexArray(mesh->vao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);

		} //End if faces > 0

		if (tcs.size() > 0)
		{
			std::vector<float> b;

			for (std::vector<Face>::iterator fit = faces.begin();
				fit != faces.end(); fit++)
			{
				b.push_back(fit->tca.x); b.push_back(fit->tca.y);
				b.push_back(fit->tcb.x); b.push_back(fit->tcb.y);
				b.push_back(fit->tcc.x); b.push_back(fit->tcc.y);
			}

			glGenBuffers(1, &vboId);

			if (!vboId)
			{
				throw std::exception();
			}

			glBindBuffer(GL_ARRAY_BUFFER, vboId);
			mesh->buffers.push_back(vboId);

			glBufferData(GL_ARRAY_BUFFER, sizeof(b.at(0)) * b.size(), &b.at(0),
				GL_STATIC_DRAW);

			glBindVertexArray(mesh->vao);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);
		}

		if (normals.size() > 0)
		{
			std::vector<float> b;

			for (std::vector<Face>::iterator fit = faces.begin();
				fit != faces.end(); fit++)
			{
				b.push_back(fit->na.x); b.push_back(fit->na.y); b.push_back(fit->na.z);
				b.push_back(fit->nb.x); b.push_back(fit->nb.y); b.push_back(fit->nb.z);
				b.push_back(fit->nc.x); b.push_back(fit->nc.y); b.push_back(fit->nc.z);
			}

			glGenBuffers(1, &vboId);

			if (!vboId)
			{
				throw std::exception();
			}

			glBindBuffer(GL_ARRAY_BUFFER, vboId);
			mesh->buffers.push_back(vboId);

			glBufferData(GL_ARRAY_BUFFER, sizeof(b.at(0)) * b.size(), &b.at(0),
				GL_STATIC_DRAW);

			glBindVertexArray(mesh->vao);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(2);
			glBindVertexArray(0);
		}
		meshes.push_back(mesh);
	}//End creation of object

}

void OBJModel::loadModel(const std::string& path)
{
	std::string currentLine = path;

	try
	{
		return loadModel(path, currentLine);
	}
	catch (std::exception& e)
	{
		std::cout << "Failed to parse model data [" + currentLine + "]"
			<< std::endl;

		throw std::exception();
		//throw Exception("Failed to parse model data [" + currentLine + "]");
	}
}

OBJModel::OBJModel(std::string objPath, std::shared_ptr<Shader> defaultShader)
{
	loadModel(objPath);

	for (int i = 0; i < meshes.size(); i++) 
	{
		meshes.at(i)->material->SetShader(defaultShader);
	}
}

