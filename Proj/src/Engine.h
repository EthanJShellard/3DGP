#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "glm/glm.hpp";
#include "glm/ext.hpp";
#include <exception>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class Engine
{
private:
	SDL_Window* window;

	void Initialise();

	/// <summary>
	/// Generate a Vertex Array Object on the GPU.
	/// </summary>
	/// <returns>VAO ID</returns>
	GLuint CreateVAO();

	/// <summary>
	/// Create VBO (Single Buffer) using GL_ARRAY_BUFFER and GL_STATIC_DRAW.
	/// </summary>
	/// <param name="data">= Raw data</param>
	/// <param name="length">= Length of data</param>
	/// <returns>VBO ID</returns>
	GLuint CreateVBO(const GLfloat * data, int length);

	/// <summary>
	/// Creates an example vertex shader, purely for testing.
	/// </summary>
	/// <returns>Example Vertex Shader ID</returns>
	GLuint CreateExampleVertexShader();

	/// <summary>
	/// Creates an example fragment shader, purely for testing.
	/// </summary>
	/// <returns>Example Fragment Shader ID</returns>
	GLuint CreateExampleFragmentShader();

	void Update();
	void Render();
public:
	int Run();

	Engine();
	~Engine();
};
