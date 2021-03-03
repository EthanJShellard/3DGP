#pragma once
#include "GL/glew.h"

#include <memory>
#include <vector>

//Forward Declaration
class VertexBuffer;


class VertexArray 
{
public:
	void SetBuffer(std::shared_ptr<VertexBuffer> buffer, int position);
	
	GLuint GetID();

	VertexArray();
private:
	GLuint id;
	bool dirty;
	std::vector< std::shared_ptr<VertexBuffer> > buffers;
};