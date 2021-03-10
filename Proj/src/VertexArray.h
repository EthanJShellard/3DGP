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
	void SetBuffer(std::shared_ptr<VertexBuffer> buffer, int position);
	
	GLuint GetID();
	size_t GetVertCount();

	VertexArray();
	VertexArray(std::string pathToModel);
	~VertexArray();
private:
	GLuint id;
	size_t vertCount;
	bool dirty;
	std::vector< std::shared_ptr<VertexBuffer> > buffers;
};