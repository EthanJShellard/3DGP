#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <bugl.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#define STB_IMAGE_IMPLEMENTATION //Needs to be defined before the include in exacly one compilation unit
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <exception>
#include <memory>

#include "Engine.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h";
#include "PostProcessShader.h"
#include "OBJModel.h"
#include "GameObject.h"
#include "GameObjectOBJ.h"
#include "LoneQuad.h"
#include "ScreenQuad.h"
#include "Scene.h"
#include "SceneLoader.h"

#include "scripted_objects/SpinningLight.h"

/// <summary>
/// Currently unused as it was found to be unnecessary - should be removed if this doesn't change
/// </summary>
/// <param name="target"></param>
/// <returns></returns>
int NearestPowerOf2(int target) 
{
	int pow = 128;
	while (pow < target) 
	{
		pow *= 2;
	}
	return pow;
}

void Engine::Initialise()
{
	std::cout << "Initliaze SDL Video..." << std::endl;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	std::cout << "Done!" << std::endl;
	std::cout << "Initliaze SDL Window..." << std::endl;
	
	//Set multisample attributes to enable antialiasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	window = SDL_CreateWindow("Ethan Shellard OpenGL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	std::cout << "Done!" << std::endl;

	std::cout << "Initliaze GLEW..." << std::endl;

	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}

	std::cout << "Done!" << std::endl;

	SDL_MaximizeWindow(window);
	glEnable(GL_MULTISAMPLE);

	//Done to prevent initial flick
	//Handle window resizing
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	/////////////////////////
	SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
	input->Update();
}

void Engine::Update()
{
	//Handle delta time
	float currTime = SDL_GetTicks(); //store current time ms
	float diffTime = currTime - lastTime; //get diff between now and previous
	lastTime = currTime;
	deltaTime = diffTime / 1000; //convert to s
	//////////////////////

	//Handle window resizing
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	if (windowHeight != height || windowWidth != width) 
	{
		screenQuad->Resize(width, height);
		multisampleRenderTexture->Resize(width, height);
		postProcessingRenderTexture->Resize(width, height);
		blurRenderTexture->Resize(width, height);
		lightKeyRenderTexture->Resize(width, height);
		outputRenderTexture->Resize(width, height);
	}
	
	windowWidth = width;
	windowHeight = height;
	/////////////////////////

	if (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)
	{
		SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
		input->ClearMousePrevious(windowWidth, windowHeight);
	}
	//Moving update into the above block makes it impossible to refocus window
	input->Update();
	if (input->GetKey(SDLK_ESCAPE)) input->quit = true;
}

void Engine::Draw()
{

}

int Engine::Run()
{
	Initialise();

	std::shared_ptr<Shader> fullbrightShader = std::make_shared<Shader>("assets/shaders/FullbrightVert.txt", "assets/shaders/FullbrightFrag.txt");
	screenQuad = std::make_shared<ScreenQuad>(fullbrightShader, windowWidth, windowHeight);

	std::shared_ptr<PostProcessShader> lightKeyShader = std::make_shared<PostProcessShader>("assets/shaders/PostProcessing/LightKeyVert.txt", "assets/shaders/PostProcessing/LightKeyFrag.txt");
	std::shared_ptr<PostProcessShader> blurShader = std::make_shared<PostProcessShader>("assets/shaders/PostProcessing/BlurVert.txt", "assets/shaders/PostProcessing/BlurFrag.txt");
	std::shared_ptr<PostProcessShader> combineShader = std::make_shared<PostProcessShader>("assets/shaders/PostProcessing/CombineVert.txt", "assets/shaders/PostProcessing/CombineFrag.txt");

	//Create RenderTextures
	multisampleRenderTexture = std::make_shared<MultisampleRenderTexture>(windowWidth, windowHeight, 8);
	postProcessingRenderTexture = std::make_shared<RenderTexture>(windowWidth, windowHeight);
	lightKeyRenderTexture = std::make_shared<RenderTexture>(windowWidth, windowHeight);
	blurRenderTexture = std::make_shared<RenderTexture>(windowWidth, windowHeight);
	outputRenderTexture = std::make_shared<RenderTexture>(windowWidth, windowHeight);

	//Enable backface culling
	glEnable(GL_CULL_FACE);

	//Enable alpha blending, allowing transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Method of alpha blending
	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//Performance metrics
	bool quit = false;
	float frameTimeSum = .0f;
	int frameCounter = 0;
	float minimumFrameTime = 60.0f;
	float maximumFrameTime = 0.0f;

	std::shared_ptr<Scene> mainScene = SceneLoader::LoadScene(1, input);
	mainScene->Start();

	//UPDATE
	while (!input->quit)
	{
		Update();
		mainScene->Update(deltaTime);
		glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, 0.0f, 1.0f);

		//Set clear colour to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		//DRAW SCENE INTO MULTISAMPLED RENDER TEXTURE
		multisampleRenderTexture->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainScene->Draw(windowWidth, windowHeight);
		multisampleRenderTexture->Unbind();
		/////////////////////////////////////////////

		multisampleRenderTexture->BlitTo(postProcessingRenderTexture);

		//Apply lightKey
		RenderTexture::RenderFromTo(postProcessingRenderTexture, lightKeyRenderTexture, lightKeyShader, screenQuad, projection);
		//Apply blur to lightkey
		RenderTexture::RenderFromTo(lightKeyRenderTexture, blurRenderTexture, blurShader, screenQuad, projection);
		//Combine blurred lightkey with output image
		RenderTexture::Combine(postProcessingRenderTexture, blurRenderTexture, outputRenderTexture, combineShader, screenQuad, projection);

		//DRAW PROCESSED RENDER TEXTURE TO SCREEN
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0,0, windowWidth, windowHeight); //Make sure to set viewport
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, outputRenderTexture->GetTextureID());
		screenQuad->Draw(projection);
		//////////////////////////////////////////////

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		//Swap opengl memory buffer and screen buffer to eliminate flicker
		SDL_GL_SwapWindow(window);

		//Performance monitoring
		frameCounter++;
		frameTimeSum += deltaTime;
		if (deltaTime < minimumFrameTime) minimumFrameTime = deltaTime;
		if (deltaTime > maximumFrameTime) maximumFrameTime = deltaTime;
	}

	SDL_Quit();

	std::cout << "Total Frames: " << frameCounter << std::endl;
	std::cout << "Average Frame Time: " << frameTimeSum / frameCounter << std::endl;
	std::cout << "Average Frame Rate: " << 1 / (frameTimeSum / frameCounter) << std::endl;
	std::cout << "Minimum Frame Time: " << minimumFrameTime << std::endl;
	std::cout << "Maximum Frame Time: " << maximumFrameTime << std::endl;
	std::cout << "Press any key to exit... ";
	std::cin.get();

	return 0;
}

Engine::Engine()
{
	window = nullptr;
	lastTime = 0;
	deltaTime = 0;
	input = std::make_shared<Input>();
	windowWidth = DEFAULT_WINDOW_WIDTH;
	windowHeight = DEFAULT_WINDOW_HEIGHT;
}

Engine::~Engine()
{
	SDL_DestroyWindow(window);
}
