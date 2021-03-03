#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class VertexBuffer 
{
public:
	void Add(glm::vec2 value);
	void Add(glm::vec3 value);
	void Add(glm::vec4 value);
	void Add(GLfloat value);

	int GetComponents();
	GLuint GetID();

	VertexBuffer();
	~VertexBuffer();
private:
	GLuint id;
	int components;
	std::vector<GLfloat> data;
	bool dirty; //Used to specify whether data is yet to be uploaded to GPU
};

