#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <memory>

#include "Input.h"
#include "ScreenQuad.h"
#include "RenderTexture.h"
#include "MultisampleRenderTexture.h"
#include <memory>;

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

/// <summary>
/// Class controlling the opengl functionality of the program.
/// </summary>
class Engine
{
private:
	SDL_Window* window;
	int windowWidth;
	int windowHeight;

	float lastTime;
	float deltaTime;

	std::shared_ptr<ScreenQuad> screenQuad;
	std::shared_ptr<RenderTexture> postProcessingRenderTexture;
	std::shared_ptr<MultisampleRenderTexture> multisampleRenderTexture;

	std::shared_ptr<RenderTexture> lightKeyRenderTexture;
	std::shared_ptr<RenderTexture> blurRenderTexture;
	std::shared_ptr<RenderTexture> blurRenderTexture2;
	std::shared_ptr<RenderTexture> outputRenderTexture;

	/// <summary>
	/// Ready the engine for operation. Creates the SDL2 and OpenGL environments. 
	/// </summary>
	void Initialise();

	/// <summary>
	/// Update engine information like input and window size
	/// </summary>
	void Update();

public:
	/// <summary>
	/// Entry point for engine. Call this function to start the engine and game.
	/// </summary>
	int Run();

	std::shared_ptr<Input> input;

	Engine();
	~Engine();
};
