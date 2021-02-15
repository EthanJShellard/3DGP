#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
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
	/// <param name="data">= Raw Data</param>
	/// <returns>VBO ID</returns>
	GLuint CreateVBO(const void * data);

	void Update();
	void Render();
public:
	int Run();
};
