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
#include "OBJModel.h"
#include "GameObject.h"
#include "GameObjectOBJ.h"
#include "LoneQuad.h"
#include "Scene.h"

void Engine::Initialise()
{
	std::cout << "Initliaze SDL Video..." << std::endl;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	std::cout << "Done!" << std::endl;


	std::cout << "Initliaze SDL Window..." << std::endl;

	window = SDL_CreateWindow("OpenGL Template",
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

GLuint Engine::CreateTexture(unsigned char* data, int width, int height)
{
	//Create and bind texture
	GLuint texID = 0;
	glGenTextures(1, &texID);

	if (!texID)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, texID);

	//Upload image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//Generate MipMap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

unsigned char* Engine::LoadTextureData(const char* file, int* width, int* height)
{
	unsigned char* data = stbi_load(file, width, height, NULL, 4);

	if (!data) throw std::exception();

	return data;
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

	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/test/vert.txt", "assets/shaders/test/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");

	std::shared_ptr<OBJModel> dust2 = std::make_shared<OBJModel>("assets/models/dust 2/triangulated.obj", program);
	std::shared_ptr<GameObjectOBJ> dust2Obj = std::make_shared<GameObjectOBJ>();
	dust2Obj->SetModel(dust2);
	dust2Obj->Rotate(-90.0f, glm::vec3(1,0,0));
	dust2Obj->SetPosition(5.0f, 1.0f, 1.0f);
	dust2Obj->SetScale(0.05f, 0.05f, 0.05f);
	
	//std::shared_ptr<OBJModel> skullModel = std::make_shared<OBJModel>("assets/models/Skull/12140_Skull_v3_L2.obj", program);
	//std::shared_ptr<GameObjectOBJ> skull = std::make_shared<GameObjectOBJ>();
	//skull->SetModel(skullModel);
	//skull->SetPosition(-5.0f, 1.0f, 1.0f);
	//skull->Rotate(-90.0f, glm::vec3(1,0,0));
	//skull->SetScale(0.05f, 0.05f, 0.05f);

	std::shared_ptr<LoneQuad> floorQuad = std::make_shared<LoneQuad>("assets/textures/Potato.jpg", program);
	floorQuad->SetScale(50.0f, 1.0f, 50.0f);
	floorQuad->SetPosition(-25.0f, 0.0f, -25.0f);

	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(input);
	mainScene->AddObject(dust2Obj);
	mainScene->AddObject(floorQuad);
	mainScene->AddLight(std::make_shared<Light>(glm::vec3(0,10,0), glm::vec3(1,1,1) ) );
	mainScene->mainCamera.transform.SetPosition(glm::vec3(0,10,0));

	//Enable backface culling
	glEnable(GL_CULL_FACE);

	//Enable alpha blending, allowing transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Method of alpha blending
	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	bool quit = false;

	//UPDATE
	while (!input->quit)
	{
		Update();
		mainScene->Update(deltaTime);

		//Set clear colour to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear
		glClear(GL_COLOR_BUFFER_BIT);

		mainScene->Draw(windowWidth, windowHeight);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		//Swap opengl memory buffer and screen buffer to eliminate flicker
		SDL_GL_SwapWindow(window);
		//clear depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	SDL_Quit();
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
