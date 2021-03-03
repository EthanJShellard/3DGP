#include "VertexArray.h"
#include "VertexBuffer.h"

#include <exception>

void VertexArray::SetBuffer(std::shared_ptr<VertexBuffer> buffer, int position)
{
	buffers.at(position) = buffer;
	dirty = true; //Data has changed and so needs to be uploaded
}

GLuint VertexArray::GetID()
{
	if (dirty) 
	{
		glBindVertexArray(id);

		for (size_t i = 0; i < buffers.size(); i++) 
		{
			if (!buffers.at(i)) continue; //If null go to next

			glBindBuffer(GL_ARRAY_BUFFER, buffers.at(i)->GetID());
			glVertexAttribPointer(i, buffers.at(i)->GetComponents(), GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(i);
		}

		//cleanup
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		dirty = false;
	}
	
	return id;
}

VertexArray::VertexArray()
{
	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &id);
	if (!id)
	{
		throw std::exception();
	}

	buffers.resize(20);

	dirty = true;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}
