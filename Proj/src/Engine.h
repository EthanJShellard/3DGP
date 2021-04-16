#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <memory>

#include "Input.h"
#include "ScreenQuad.h"
#include <memory>;

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

	std::shared_ptr<ScreenQuad> screenQuad;

	void Initialise();

	void Update();
	void Draw();
public:
	int Run();

	std::shared_ptr<Input> input;

	Engine();
	~Engine();
};
