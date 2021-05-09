#pragma once
#include "GL/glew.h"

#include <memory>
#include <vector>
#include <string>

//Forward Declaration
class VertexBuffer;

//Wrapper for vertex array objects. Will bind buffers lazily, when GetID() is called.
class VertexArray 
{
public:
	/// <summary>
	/// Set a vertex buffer on this VBO. This will make the Vertex Array dirty
	/// </summary>
	void SetBuffer(std::shared_ptr<VertexBuffer> buffer, int position);
	//Bind buffers if they've changed and then return ID
	GLuint GetID();
	size_t GetVertCount();
	void SetVertCount(size_t count);

	VertexArray();
	~VertexArray();
private:
	GLuint id;
	size_t vertCount;
	bool dirty;
	std::vector< std::shared_ptr<VertexBuffer> > buffers;
};