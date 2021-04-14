#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <memory>

#include "Input.h"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

class Engine
{
private:
	SDL_Window* window;
	int windowWidth;
	int windowHeight;

	float lastTime;
	float deltaTime;

	void Initialise();

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
	void Draw();
public:
	int Run();

	std::shared_ptr<Input> input;

	Engine();
	~Engine();
};
