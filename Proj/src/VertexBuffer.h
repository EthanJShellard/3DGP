#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

//Wrapper for vertex buffer objects.
class VertexBuffer 
{
public:
	void Add(glm::vec2 value);
	void Add(float x, float y);
	void Add(glm::vec3 value);
	void Add(float x, float y, float z);
	void Add(glm::vec4 value);
	void Add(GLfloat value);

	//Get the number of components in one unit of data for this buffer. (e.g. 3 for glm::vec3)
	int GetComponents();
	// Get the location of this vertex buffer. Uploads data to GPU if it has changed since last call.
	GLuint GetID();

	VertexBuffer();
	~VertexBuffer();
	void Delete();
private:
	GLuint id;
	int components;
	std::vector<GLfloat> data;
	bool dirty; //Used to specify whether data is yet to be uploaded to GPU
	bool dead;
};

