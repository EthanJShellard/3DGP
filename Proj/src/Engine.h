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

//Class controlling the opengl functionality of the program.
class Engine
{
private:
	SDL_Window* m_window;
	int m_windowWidth;
	int m_windowHeight;

	float m_lastTime;
	float m_deltaTime;

	std::shared_ptr<ScreenQuad> m_screenQuad;
	std::shared_ptr<RenderTexture> m_postProcessingRenderTexture;
	std::shared_ptr<MultisampleRenderTexture> m_multisampleRenderTexture;

	std::shared_ptr<RenderTexture> m_lightKeyRenderTexture;
	std::shared_ptr<RenderTexture> m_blurRenderTexture;
	std::shared_ptr<RenderTexture> m_blurRenderTexture2;
	std::shared_ptr<RenderTexture> m_outputRenderTexture;

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

	std::shared_ptr<Input> m_input;

	Engine();
	~Engine();
};
