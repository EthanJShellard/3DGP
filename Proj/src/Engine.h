#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

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

	/// <summary>
	/// Upload texture data to the GPU in RGBA format
	/// </summary>
	/// <param name="data">Texture data</param>
	/// <param name="width">Width of the texture</param>
	/// <param name="height">Height of the texture</param>
	/// <returns></returns>
	GLuint CreateTexture(unsigned char* data, int width, int height);

	/// <summary>
	/// Load Texture data from file
	/// </summary>
	/// <param name="file">Location + Filename</param>
	/// <param name="width">Storage for width of image</param>
	/// <param name="height">Storage for height of image</param>
	/// <returns></returns>
	unsigned char* LoadTextureData(const char* file,  int* width, int* height);

	void Update();
	void Render();
public:
	int Run();

	Engine();
	~Engine();
};
