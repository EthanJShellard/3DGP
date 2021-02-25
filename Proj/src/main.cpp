#pragma once
#include "Engine.h"
#include <exception>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void AllInOne()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	SDL_Window* window = SDL_CreateWindow("OpenGL Template",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}
	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}

	bool quit = false;

	//create primitive shape data
	const GLfloat positions[] =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	const GLfloat colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	/*----------------------------------------------------------------*/
	//SET UP  VAO
	/*-----------------------------------------------------------------*/
	GLuint vaoId = 0;

	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &vaoId);

	if (!vaoId)
	{
		throw std::exception();
	}

	/*-----------------------------------------------------------------*/
	//SET UP POSITIONS VBO
	/*-----------------------------------------------------------------*/
	GLuint positionsVboId = 0;

	//Create a new VBO on the GPU and bind it (Gives us a 'reference' in positionsVboId)
	//1 is number of buffers to initialise - usually just use 1
	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
	{
		throw std::exception();
	}

	//Now when we operate, we operate on this buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) * 9, positions, GL_STATIC_DRAW);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*-----------------------------------------------------------------*/
	//SET UP COLORS VBO
	/*-----------------------------------------------------------------*/
	GLuint colorsVboId = 0;

	//Create a volors VBO on the GPU and bind it
	glGenBuffers(1, &colorsVboId);

	if (!colorsVboId)
	{
		throw std::exception();
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);

	//Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors) * 12, colors, GL_STATIC_DRAW);

	//Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*-----------------------------------------------------------------*/
	//BIND VBOS TO VAOS
	/*-----------------------------------------------------------------*/
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/*-----------------------------------------------------------------*/
	//CREATE SHADERS \/\/\/
	/*-----------------------------------------------------------------*/

	const GLchar* vertexShaderSrc =
		"uniform mat4 u_Projection;           " \
		"uniform mat4 u_Model;                " \
		"attribute vec3 a_Position;             " \
		"attribute vec4 a_Color;                " \
		"                                       " \
		"varying vec4 v_Color;                  " \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0); " \
		" v_Color = a_Color;                   " \
		"}                                      ";

	// Create a new vertex shader, attach source code, compile it and
	// check for errors.
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}

	const GLchar* fragmentShaderSrc =
		"varying vec4 v_Color;             " \
		"uniform float u_Pulse;            " \
		"                                  " \
		"void main()                       " \
		"{                                 " \
		" gl_FragColor = v_Color * u_Pulse;" \
		"}                                 ";

	// Create a new fragment shader, attach source code, compile it and
	// check for errors.
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}

	/*-----------------------------------------------------------------*/
	//CREATE PROGRAM AND ATTACH SHADER OBJECTS
	/*-----------------------------------------------------------------*/

	// Create new shader program and attach our shader objects
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glBindAttribLocation(programId, 0, "a_Position");
	glBindAttribLocation(programId, 1, "a_Color");

	// Perform the link and check for failure
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}


	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	// Store location of uniforms and check if successfully found
	GLint pulseUniformId = glGetUniformLocation(programId, "u_Pulse");
	GLint modelLoc = glGetUniformLocation(programId, "u_Model");
	GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");
	if (pulseUniformId == -1 || modelLoc == -1 || projectionLoc == -1)
	{
		throw std::exception();
	}

	/*-----------------------------------------------------------------*/
	//BIND SHADER PROGRAM AND DRAW TRIANGLE
	/*-----------------------------------------------------------------*/

	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(programId);
	glBindVertexArray(vaoId);

	// Draw 3 vertices (a triangle)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);

	float pulse = 0;
	float delta = 0.0001f;
	float angle = 0;


	while (!quit)
	{
		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//Handle window resizing
		int width = 0;
		int height = 0;
		SDL_GetWindowSize(window, &width, &height);
		glViewport(0,0,width,height);

		//Handle pulse uniform
		pulse += delta;
		if (pulse > 1.0f || pulse <= 0.0f) delta = -delta;
		glUseProgram(programId);
		glUniform1f(pulseUniformId, pulse);
		glUseProgram(0);

		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)width / (float)height, 0.1f, 100.f);

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), -4.5f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

		// Increase the float angle so next frame the triangle rotates further
		angle += 0.02f;

		// Make sure the current program is bound
		glUseProgram(programId);

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
			glm::value_ptr(projection));

		//Set clear colour to red
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear
		glClear(GL_COLOR_BUFFER_BIT);

		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(programId);
		glBindVertexArray(vaoId);

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		//Swap opengl memory buffer and screen buffer to eliminate flicker
		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	(new Engine())->Run();
	return 0;
}