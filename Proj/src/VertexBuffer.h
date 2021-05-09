#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

//Wrapper for vertex buffer objects.
class VertexBuffer 
{
public:
	void Add(glm::vec2 _value);
	void Add(float _x, float _y);
	void Add(glm::vec3 _value);
	void Add(float _x, float _y, float _z);
	void Add(glm::vec4 _value);
	void Add(GLfloat _value);

	//Get the number of components in one unit of data for this buffer. (e.g. 3 for glm::vec3)
	int GetComponents();
	// Get the location of this vertex buffer. Uploads data to GPU if it has changed since last call.
	GLuint GetID();

	VertexBuffer();
	~VertexBuffer();
	void Delete();
private:
	GLuint m_id;
	int m_components;
	std::vector<GLfloat> m_data;
	bool m_dirty; //Used to specify whether data is yet to be uploaded to GPU
	bool m_dead;
};

