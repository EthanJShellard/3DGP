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


void Engine::Initialise()
{
	std::cout << "Initliaze SDL Video..." << std::endl;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	std::cout << "Done!" << std::endl;
	std::cout << "Initialize SDL Window..." << std::endl;

	m_window = SDL_CreateWindow("Ethan Shellard OpenGL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(m_window))
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

	SDL_MaximizeWindow(m_window);

	//Done to prevent initial flick
	//Handle window resizing
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(m_window, &width, &height);
	glViewport(0, 0, width, height);
	m_windowWidth = width;
	m_windowHeight = height;
	/////////////////////////
	SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
	m_input->Update();
}

void Engine::Update()
{
	//Handle delta time
	float currTime = SDL_GetTicks(); //store current time ms
	float diffTime = currTime - m_lastTime; //get diff between now and previous
	m_lastTime = currTime;
	m_deltaTime = diffTime / 1000; //convert to s
	//////////////////////

	//Handle window resizing
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(m_window, &width, &height);
	glViewport(0, 0, width, height);

	//If window has been resized, adjust screen quad and all render textures to fit the new dimensions
	if (m_windowHeight != height || m_windowWidth != width)
	{
		m_screenQuad->Resize(width, height);
		m_multisampleRenderTexture->Resize(width, height);
		m_postProcessingRenderTexture->Resize(width, height);
		m_blurRenderTexture->Resize(width, height);
		m_blurRenderTexture2->Resize(width, height);
		m_lightKeyRenderTexture->Resize(width, height);
		m_outputRenderTexture->Resize(width, height);

		m_windowWidth = width;
		m_windowHeight = height;
	}
	/////////////////////////

	//Stop the program from grabbing the mouse when the user has unfocused the window
	if (SDL_GetWindowFlags(m_window) & SDL_WINDOW_INPUT_FOCUS)
	{
		SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
		m_input->ClearMousePrevious(m_windowWidth, m_windowHeight);
	}
	m_input->Update();
	if (m_input->GetKey(SDLK_ESCAPE)) m_input->m_quit = true;
}


int Engine::Run()
{
	Initialise();

	//Create full bright shader mostly used to display textures without lighting
	std::shared_ptr<Shader> fullbrightShader = std::make_shared<Shader>("assets/shaders/FullbrightVert.txt", "assets/shaders/FullbrightFrag.txt");

	m_screenQuad = std::make_shared<ScreenQuad>(fullbrightShader, m_windowWidth, m_windowHeight);

	//Postprocessing shaders
	std::shared_ptr<PostProcessShader> lightKeyShader = std::make_shared<PostProcessShader>("assets/shaders/PostProcessing/LightKeyVert.txt", "assets/shaders/PostProcessing/LightKeyFrag.txt");
	std::shared_ptr<PostProcessShader> blurShader = std::make_shared<PostProcessShader>("assets/shaders/PostProcessing/BlurVert.txt", "assets/shaders/PostProcessing/BlurFrag.txt");
	std::shared_ptr<PostProcessShader> combineShader = std::make_shared<PostProcessShader>("assets/shaders/PostProcessing/CombineVert.txt", "assets/shaders/PostProcessing/CombineFrag.txt");

	//Create RenderTextures
	m_multisampleRenderTexture = std::make_shared<MultisampleRenderTexture>(m_windowWidth, m_windowHeight, 8);
	m_postProcessingRenderTexture = std::make_shared<RenderTexture>(m_windowWidth, m_windowHeight);
	m_lightKeyRenderTexture = std::make_shared<RenderTexture>(m_windowWidth, m_windowHeight);
	m_blurRenderTexture = std::make_shared<RenderTexture>(m_windowWidth, m_windowHeight);
	m_blurRenderTexture2 = std::make_shared<RenderTexture>(m_windowWidth, m_windowHeight);
	m_outputRenderTexture = std::make_shared<RenderTexture>(m_windowWidth, m_windowHeight);

	//Loading screen texture
	std::shared_ptr<Texture> loadingTexture = std::make_shared<Texture>("assets/textures/Loading.png");

	//Enable backface culling
	glEnable(GL_CULL_FACE);

	//Enable alpha blending, allowing transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Method of alpha blending
	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//Performance metrics
	float frameTimeSum = .0f;
	int frameCounter = 0;
	float minimumFrameTime = 60.0f;
	float maximumFrameTime = 0.0f;

	//Create main scene, loading into the default first scene
	std::shared_ptr<Scene> mainScene = SceneLoader::LoadScene(0, m_input);

	mainScene->Start();

	int currentScene = 0;
	int nextScene = 0;

	//Set clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//UPDATE
	while (!m_input->m_quit)
	{
		Update();
		//Update orthographic projectiona fter potential screen resize
		glm::mat4 projection = glm::ortho(0.0f, (float)m_windowWidth, 0.0f, (float)m_windowHeight, 0.0f, 1.0f);

		//Handle scene changes //////////////
		if (m_input->GetKey(SDLK_1)) nextScene = 0;
		else if (m_input->GetKey(SDLK_2)) nextScene = 1;
		else if (m_input->GetKey(SDLK_3)) nextScene = 2;
		else if (m_input->GetKey(SDLK_4)) nextScene = 3;

		if (nextScene != currentScene)
		{
			//Show loading screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, m_windowWidth, m_windowHeight);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, loadingTexture->m_id);
			m_screenQuad->Draw(projection);
			SDL_GL_SwapWindow(m_window);
			glBindTexture(GL_TEXTURE_2D, 0);

			mainScene.reset();

			mainScene = SceneLoader::LoadScene(nextScene, m_input);

			currentScene = nextScene;
			mainScene->Start();

		}
		///////////////////////////////////////
		
		mainScene->Update(m_deltaTime);


		//DRAW SCENE INTO MULTISAMPLED RENDER TEXTURE
		m_multisampleRenderTexture->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainScene->Draw(m_windowWidth, m_windowHeight);
		m_multisampleRenderTexture->Unbind();
		/////////////////////////////////////////////

		//Blit multisampled texture into a standard render texture.
		m_multisampleRenderTexture->BlitTo(m_postProcessingRenderTexture);

		//Apply lightKey
		RenderTexture::RenderFromTo(m_postProcessingRenderTexture, m_lightKeyRenderTexture, lightKeyShader, m_screenQuad, projection);
		//Apply blur to lightkey
		RenderTexture::RenderFromTo(m_lightKeyRenderTexture, m_blurRenderTexture, blurShader, m_screenQuad, projection);
		RenderTexture::RenderFromTo(m_blurRenderTexture, m_blurRenderTexture2, blurShader, m_screenQuad, projection);
		RenderTexture::RenderFromTo(m_blurRenderTexture2, m_blurRenderTexture, blurShader, m_screenQuad, projection);
		RenderTexture::RenderFromTo(m_blurRenderTexture, m_blurRenderTexture2, blurShader, m_screenQuad, projection);
		RenderTexture::RenderFromTo(m_blurRenderTexture2, m_blurRenderTexture, blurShader, m_screenQuad, projection);
		RenderTexture::RenderFromTo(m_blurRenderTexture, m_blurRenderTexture2, blurShader, m_screenQuad, projection);
		//Combine blurred lightkey with output image
		RenderTexture::Combine(m_postProcessingRenderTexture, m_blurRenderTexture2, m_outputRenderTexture, combineShader, m_screenQuad, projection);

		//DRAW PROCESSED RENDER TEXTURE TO SCREEN
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_windowWidth, m_windowHeight); //Make sure to set viewport
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_outputRenderTexture->GetTextureID());
		m_screenQuad->Draw(projection);
		//////////////////////////////////////////////

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Swap opengl memory buffer and screen buffer to eliminate flicker
		SDL_GL_SwapWindow(m_window);

		//Performance monitoring
		frameCounter++;
		frameTimeSum += m_deltaTime;
		if (m_deltaTime < minimumFrameTime) minimumFrameTime = m_deltaTime;
		if (m_deltaTime > maximumFrameTime) maximumFrameTime = m_deltaTime;
	}

	//Close SDL instance
	SDL_Quit();

	//Deleting scene now for debug output purposes
	mainScene.reset();

	std::cout << "\n\nPERFORMANCE METRICS (Includes loading times):\nTotal Frames: " << frameCounter << std::endl;
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
	m_window = nullptr;
	m_lastTime = 0;
	m_deltaTime = 0;
	m_input = std::make_shared<Input>();
	m_windowWidth = DEFAULT_WINDOW_WIDTH;
	m_windowHeight = DEFAULT_WINDOW_HEIGHT;
}

Engine::~Engine()
{
	SDL_DestroyWindow(m_window);
}
