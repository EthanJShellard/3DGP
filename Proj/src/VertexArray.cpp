#include <bugl.h>
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <iostream>

#include <exception>

void VertexArray::SetBuffer(std::shared_ptr<VertexBuffer> buffer, int position)
{
	if (buffers.size() <= position) buffers.resize(position + 1);
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

size_t VertexArray::GetVertCount()
{
	return vertCount;
}

void VertexArray::SetVertCount(size_t count)
{
	vertCount = count;
}

VertexArray::VertexArray()
{
	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &id);
	if (!id)
	{
		throw std::exception();
	}

	buffers.resize(3); //to save on resizes. Our default size will be 3

	vertCount = 0;
	dirty = true;
}

VertexArray::~VertexArray()
{
	std::cout << "Deleting buffers ";

	//Delete your buffers
	for (int i = 0; i < buffers.size(); i++) 
	{
		std::cout << buffers.at(i)->GetID() << " ";
		buffers.at(i)->Delete();
	}
	std::cout << "\nDeleting vertex arrays " << id << std::endl;
	glDeleteVertexArrays(1, &id);
}
