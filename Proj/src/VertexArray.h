#pragma once
#include "GL/glew.h"

#include <memory>
#include <vector>
#include <string>

//Forward Declaration
class VertexBuffer;


class VertexArray 
{
public:
	/// <summary>
	/// Set a vertex buffer on this VBO. This will make the Vertex Array dirty
	/// </summary>
	void SetBuffer(std::shared_ptr<VertexBuffer> buffer, int position);
	
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