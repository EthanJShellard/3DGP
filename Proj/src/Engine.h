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
	/// Read the engine for operation. 
	/// </summary>
	void Initialise();

	void Update();
	void Draw();
public:
	int Run();

	std::shared_ptr<Input> input;

	Engine();
	~Engine();
};
