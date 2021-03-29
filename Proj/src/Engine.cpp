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

void Engine::Initialise()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	window = SDL_CreateWindow("OpenGL Template",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}
	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}

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

	SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
	input->ClearMousePrevious(windowWidth, windowHeight);
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
	unsigned char* data = LoadTextureData("assets/models/curuthers/Whiskers_diffuse.png", &width, &height);
	GLint textureID = CreateTexture(data, width, height);

	std::shared_ptr<OBJModel> dust2 = std::make_shared<OBJModel>("assets/models/Dust 2/Triangulated.obj");
	for (int i = 0; i < dust2->objects.size(); i++)
	{
		std::cout << dust2->objects.at(i)->vertexCount << std::endl;
	}

	std::shared_ptr<VertexBuffer> textureCoordsVBO = std::make_shared<VertexBuffer>();
	textureCoordsVBO->Add(glm::vec2(0.5f, 1.0f));
	textureCoordsVBO->Add(glm::vec2(0.0f, 0.0f));
	textureCoordsVBO->Add(glm::vec2(1.0f, 0.0f));

	std::shared_ptr<VertexBuffer> positionsVBO = std::make_shared<VertexBuffer>();
	positionsVBO->Add(glm::vec3(0.0f, 0.5f, 0.0f));
	positionsVBO->Add(glm::vec3(-0.5f, -0.5f, 0.0f));
	positionsVBO->Add(glm::vec3(0.5f, -0.5f, 0.0f));

	//Create VAO and set buffers in VAO
	std::shared_ptr<VertexArray> VAO = std::make_shared<VertexArray>();
	VAO->SetBuffer(positionsVBO, 0);
	VAO->SetBuffer(textureCoordsVBO, 1);

	//CREATE CAT
	std::shared_ptr<VertexArray> cat = std::make_shared<VertexArray>("assets/models/curuthers/curuthers.obj");

	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/test/vert.txt", "assets/shaders/test/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");

	// Store location of uniforms and check if successfully found
	GLint texLoc = glGetUniformLocation(program->GetID(), "u_Texture");
	GLint modelLoc = glGetUniformLocation(program->GetID(), "u_Model");
	GLint projectionLoc = glGetUniformLocation(program->GetID(), "u_Projection");
	GLint viewLoc = glGetUniformLocation(program->GetID(), "u_View");
	GLint camPosLoc = glGetUniformLocation(program->GetID(), "u_camPos");
	if (texLoc == -1 || modelLoc == -1 || projectionLoc == -1 || viewLoc == -1 || camPosLoc == -1)
	{
		throw std::exception();
	}
	/////////////////////////////////////////////////////////////////////////

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);

	glm::vec3 position = glm::vec3(0);
	glm::mat4 camRot = glm::mat4(1);
	glm::vec3 rot = glm::vec3(0);
	GLfloat camSens = 0.1f;

	float delta = 0.01f;
	float angle = 0;
	glm::mat4 view = glm::mat4(1.0f);
	
	

	//Bind the texture we loaded in
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

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

		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)windowWidth / (float)windowHeight, 0.1f, 100.f);

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(/*glm::sin(glm::radians(angle))*/ 0, glm::cos(glm::radians(angle)) -5, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1,0,0));
		model = glm::rotate(model, glm::radians(angle * 0), glm::vec3(0, 1, 0));

		// Increase the float angle so next frame the model rotates further
		angle += 10 * deltaTime;

		// Make sure the current program is bound
		glUseProgram(program->GetID());

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Upload the view matrix after movement and rotation
		view = glm::mat4(1);

		if (input->GetKey(SDLK_w)) position.z -= 10 * deltaTime;
		if (input->GetKey(SDLK_s)) position.z += 10 * deltaTime;
		if (input->GetKey(SDLK_a)) position.x -= 10 * deltaTime;
		if (input->GetKey(SDLK_d)) position.x += 10 * deltaTime;
		
		view = glm::translate(view, position);

		rot.x -= input->GetMouseDelta().x * camSens;
		rot.y -= input->GetMouseDelta().y * camSens;
		view = glm::rotate(view, glm::radians(rot.x), glm::vec3(0, 1, 0));
		view = glm::rotate(view, glm::radians(rot.y), glm::vec3(1, 0, 0));


		glUniform3f(camPosLoc, position.x, position.y, position.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(view)));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
			glm::value_ptr(projection));


		//Set clear colour to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear
		glClear(GL_COLOR_BUFFER_BIT);

		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(program->GetID());
		glBindVertexArray(cat->GetID());

		// Draw model
		//glDrawArrays(GL_TRIANGLES, 0, cat->GetVertCount());

		for (int i = 0; i < dust2->objects.size(); i++) 
		{
			glBindVertexArray(dust2->objects.at(i)->vao);
			glBindTexture(GL_TEXTURE_2D, dust2->objects.at(i)->material->texture);
			glDrawArrays(GL_TRIANGLES, 0, dust2->objects.at(i)->vertexCount);
		}

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
