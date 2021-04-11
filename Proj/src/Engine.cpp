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

	int width = 0;
	int height = 0;

	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/test/vert.txt", "assets/shaders/test/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");

	std::shared_ptr<OBJModel> dust2 = std::make_shared<OBJModel>("assets/models/City/Center city Sci-Fi/Center City Sci-Fi.obj", program);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
	go->SetModel(dust2);
	//go->Rotate(-90.0f, glm::vec3(1,0,0));
	go->SetPosition(.0f, -30.0f, 0.0f);
	go->SetScale(0.5f, 0.5f, 0.5f);
	
	// Store location of uniforms and check if successfully found
	GLint texLoc = glGetUniformLocation(program->GetID(), "u_Texture");
	GLint modelLoc = glGetUniformLocation(program->GetID(), "u_Model");
	GLint projectionLoc = glGetUniformLocation(program->GetID(), "u_Projection");
	GLint viewLoc = glGetUniformLocation(program->GetID(), "u_View");
	GLint camPosLoc = glGetUniformLocation(program->GetID(), "u_camPos");
	GLint lightPositonLocation = glGetUniformLocation(program->GetID(), "u_lightPos");
	GLint dissolveLocation = glGetUniformLocation(program->GetID(), "u_dissolve");
	if (texLoc == -1 || modelLoc == -1 || projectionLoc == -1 || viewLoc == -1 || camPosLoc == -1 || lightPositonLocation == -1 || dissolveLocation == -1)
	{
		throw std::exception();
	}

	glm::vec3 position = glm::vec3(0);
	glm::mat4 camRot = glm::mat4(1);
	glm::vec3 rot = glm::vec3(0);
	GLfloat camSens = 0.1f;

	float delta = 0.01f;
	float angle = 0;
	glm::mat4 view = glm::mat4(1.0f);
	
	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0,1,0));

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

		// Prepare the projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)windowWidth / (float)windowHeight, 0.1f, 500.f);

		//Create view matrix
		view = glm::mat4(1);

		if (input->GetKey(SDLK_w)) position.z -= 10 * deltaTime;
		if (input->GetKey(SDLK_s)) position.z += 10 * deltaTime;
		if (input->GetKey(SDLK_a)) position.x -= 10 * deltaTime;
		if (input->GetKey(SDLK_d)) position.x += 10 * deltaTime;

		if (input->GetKey(SDLK_MINUS)) go->SetScale(go->GetScale() * (1 - deltaTime));
		if (input->GetKey(SDLK_EQUALS)) go->SetScale(go->GetScale() * (1 + deltaTime));

		if (input->GetKey(SDLK_DOWN)) go->Translate(glm::vec3(0.0f, -deltaTime, 0.0f));
		if (input->GetKey(SDLK_DOWN)) go->Rotate(deltaTime * 90.0f, glm::vec3(1,0,0));
		if (input->GetKey(SDLK_UP)) go->Translate(glm::vec3(0.0f, deltaTime, 0.0f));
		if (input->GetKey(SDLK_RIGHT)) go->Translate(glm::vec3(deltaTime, 0.0f, 0.0f));
		if (input->GetKey(SDLK_LEFT)) go->Translate(glm::vec3(-deltaTime, 0.0f, 0.0f));
		
		view = glm::translate(view, position);

		rot.x -= input->GetMouseDelta().x * camSens;
		rot.y -= input->GetMouseDelta().y * camSens;
		view = glm::rotate(view, glm::radians(rot.x), glm::vec3(0, 1, 0));
		view = glm::rotate(view, glm::radians(rot.y), glm::vec3(1, 0, 0));

		//Set clear colour to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear
		glClear(GL_COLOR_BUFFER_BIT);

		//DRAW
		go->Draw(projection, glm::inverse(view), position, lightPositions);

		//ORTHOGRAPHIC DEMO#####################################################
		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)DEFAULT_WINDOW_WIDTH, 0.0f,
			(float)DEFAULT_WINDOW_HEIGHT, 0.0f, 1.0f);

		//// Prepare model matrix. The scale is important because now our triangle
		//// would be the size of a single pixel when mapped to an orthographic
		//// projection.
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(100, WINDOW_HEIGHT - 100, 0));
		//model = glm::scale(model, glm::vec3(100, 100, 1));

		//// Upload the model matrix
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//// Upload the projection matrix
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
		//	glm::value_ptr(projection));

		//// Draw shape as before
		//glBindVertexArray(VAO->GetID());
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		////############################################################################

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
