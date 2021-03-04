#include "VertexBuffer.h"
#include <exception>

void VertexBuffer::Add(glm::vec2 value)
{
	if (components != 2 && components != 0) throw std::exception();

	//Flatten data
	data.push_back(value.x);
	data.push_back(value.y);

	components = 2;
	//Data yet to be uploaded
	dirty = true;
}

void VertexBuffer::Add(glm::vec3 value)
{
	if (components != 3 && components != 0) throw std::exception();

	//Flatten data
	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);

	components = 3;
	//Data yet to be uploaded
	dirty = true;
}

void VertexBuffer::Add(glm::vec4 value)
{
	if (components != 4 && components != 0) throw std::exception();

	//Flatten data
	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	data.push_back(value.w);

	components = 4;
	//Data yet to be uploaded
	dirty = true;
}

void VertexBuffer::Add(GLfloat value)
{
	if (components != 1 && components != 0) throw std::exception();

	//Flatten data
	data.push_back(value);

	components = 1;
	//Data yet to be uploaded
	dirty = true;
}

int VertexBuffer::GetComponents()
{
	return components;
}

GLuint VertexBuffer::GetID()
{
	//We know that the data will be needed on the GPU after GETID is called
	//So we upload if the data has changed
	if (dirty) 
	{
		//Now when we operate, we operate on this buffer
		glBindBuffer(GL_ARRAY_BUFFER, id);

		// Upload a copy of the data from memory into the new VBO
		glBufferData(GL_ARRAY_BUFFER,
			data.size() * sizeof(data.at(0)), //Length of vector times size of datum
			&data.at(0), GL_STATIC_DRAW);	//Pointer to first item (data will be contiguous)

		// Reset the state
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Data now uploaded
		dirty = false;
	}

	return id;
}

VertexBuffer::VertexBuffer()
{
	//Create a new VBO on the GPU and bind it (Gives us a 'reference' in positionsVboId)
	//1 is number of buffers to initialise - usually just use 1
	glGenBuffers(1, &id);
	if (!id)
	{
		throw std::exception();
	}

	components = 0;
	//Data yet to be uploaded
	dirty = true;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}
