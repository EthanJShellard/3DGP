#include "VertexBuffer.h"
#include <exception>

void VertexBuffer::Add(glm::vec2 _value)
{
	if (m_components != 2 && m_components != 0) throw std::exception();

	//Flatten data
	m_data.push_back(_value.x);
	m_data.push_back(_value.y);

	m_components = 2;
	//Data yet to be uploaded
	m_dirty = true;
}

void VertexBuffer::Add(float _x, float _y)
{
	if (m_components != 2 && m_components != 0) throw std::exception();

	m_data.push_back(_x);
	m_data.push_back(_y);

	m_components = 2;
	//Data yet to be uploaded
	m_dirty = true;
}

void VertexBuffer::Add(glm::vec3 _value)
{
	if (m_components != 3 && m_components != 0) throw std::exception();

	//Flatten data
	m_data.push_back(_value.x);
	m_data.push_back(_value.y);
	m_data.push_back(_value.z);

	m_components = 3;
	//Data yet to be uploaded
	m_dirty = true;
}

void VertexBuffer::Add(float _x, float _y, float _z)
{
	if (m_components != 3 && m_components != 0) throw std::exception();

	m_data.push_back(_x);
	m_data.push_back(_y);
	m_data.push_back(_z);

	m_components = 3;
	//Data yet to be uploaded
	m_dirty = true;
}

void VertexBuffer::Add(glm::vec4 _value)
{
	if (m_components != 4 && m_components != 0) throw std::exception();

	//Flatten data
	m_data.push_back(_value.x);
	m_data.push_back(_value.y);
	m_data.push_back(_value.z);
	m_data.push_back(_value.w);

	m_components = 4;
	//Data yet to be uploaded
	m_dirty = true;
}

void VertexBuffer::Add(GLfloat _value)
{
	if (m_components != 1 && m_components != 0) throw std::exception();

	//Flatten data
	m_data.push_back(_value);

	m_components = 1;
	//Data yet to be uploaded
	m_dirty = true;
}

int VertexBuffer::GetComponents()
{
	return m_components;
}

GLuint VertexBuffer::GetID()
{
	//We know that the data will be needed on the GPU after GETID is called
	//So we upload if the data has changed
	if (m_dirty) 
	{
		//Now when we operate, we operate on this buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		// Upload a copy of the data from memory into the new VBO
		glBufferData(GL_ARRAY_BUFFER,
			m_data.size() * sizeof(m_data.at(0)), //Length of vector times size of datum
			&m_data.at(0), GL_STATIC_DRAW);	//Pointer to first item (data will be contiguous)

		// Reset the state
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Data now uploaded
		m_dirty = false;
	}

	return m_id;
}

VertexBuffer::VertexBuffer()
{
	//Create a new VBO
	glGenBuffers(1, &m_id);
	if (!m_id)
	{
		throw std::exception();
	}

	m_components = 0;
	//Data yet to be uploaded
	m_dirty = true;
	//Not deleted yet
	m_dead = false;
}

VertexBuffer::~VertexBuffer()
{
	if (!m_dead) 
	{
		glDeleteBuffers(1, &m_id);
	}
}

void VertexBuffer::Delete()
{
	glDeleteBuffers(1, &m_id);
	m_dead = true;
}
