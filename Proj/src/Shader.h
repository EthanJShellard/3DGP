#pragma once
#include "GL/glew.h"

//Shader wrapper with lazy compilation
class Shader 
{
public:
	void LoadNewVertexShader(const char * path);
	void LoadNewFragmentShader(const char * path);
	void BindAttribute(int index, const char * identifier);
	//Link shader program if anything has changed and return the ID
	GLuint GetID();

	Shader(const char * vertexPath, const char * fragmentPath);
	Shader();
	~Shader();
protected:
	GLuint vertID;
	GLuint fragID;
	GLuint id;

	//If attributes or shaders are changed, program will need to be relinked.
	//Dirty is used to track if there have been changes since last link.
	bool dirty;
};