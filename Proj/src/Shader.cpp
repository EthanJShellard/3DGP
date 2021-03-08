#include "GL/glew.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



void Shader::LoadNewVertexShader(const char* path)
{
	//Clean up old shader
	if(fragID)glDetachShader(id, fragID);
	if (vertID) 
	{
		glDetachShader(id, vertID);
		glDeleteShader(vertID);
	}

	std::ifstream fileRead;
	std::stringstream strStream;
	std::string stringSrc;

	fileRead.open(path);
	strStream << fileRead.rdbuf();
	stringSrc = strStream.str();
	const char* src = stringSrc.c_str();
	fileRead.close();

	// Create a new vertex shader, attach source code, compile it and
	// check for errors.
	vertID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertID, 1, &src, NULL);
	glCompileShader(vertID);
	GLint success = 0;
	glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint maxLength = 0; 
		glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertID, maxLength, &maxLength, &errorLog[0]);
		std::cout << &errorLog.at(0) << std::endl;
		throw std::exception();
	}

	dirty = true;
}

void Shader::LoadNewFragmentShader(const char* path)
{
	//Clean up old shader
	if(vertID)glDetachShader(id, vertID);
	if (fragID) 
	{
		glDetachShader(id, fragID);
		glDeleteShader(fragID);
	}

	std::ifstream fileRead;
	std::stringstream strStream;
	std::string stringSrc;

	fileRead.open(path);
	strStream << fileRead.rdbuf();
	stringSrc = strStream.str();
	const char* src = stringSrc.c_str();
	fileRead.close();

	// Create a new fragment shader, attach source code, compile it and
	// check for errors.
	fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragID, 1, &src, NULL);
	glCompileShader(fragID);
	GLint success = 0;
	glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint maxLength = 0; 
		glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragID, maxLength, &maxLength, &errorLog[0]);
		std::cout << &errorLog.at(0) << std::endl;
		throw std::exception();
	}

	dirty = true;
}

void Shader::BindAttribute(int index, const char* identifier)
{
	glBindAttribLocation(id, index, identifier);
	dirty = true;
}

GLuint Shader::GetID()
{
	if (dirty) 
	{
		GLint success = 0;
		glAttachShader(id, fragID);
		glAttachShader(id, vertID);

		// Perform the link and check for failure
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLint maxLength = 0; 
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			if(maxLength)
			{
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
				std::cout << &errorLog.at(0) << std::endl; 
				throw std::exception();
			}
			else 
			{
				throw std::exception();
			}
		}

		dirty = false;
	}

	return id;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//Initialise variables
	dirty = true;
	vertID = 0;
	fragID = 0;

	LoadNewVertexShader(vertexPath);
	LoadNewFragmentShader(fragmentPath);

	id = glCreateProgram();
}

Shader::~Shader()
{
	glDetachShader(id, vertID);
	glDeleteShader(vertID);
	glDetachShader(id, fragID);
	glDeleteShader(fragID);
	glDeleteProgram(id);
}
