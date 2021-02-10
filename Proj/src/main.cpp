#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <exception>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char* argv[])
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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
		"attribute vec3 a_Position;             " \
		"attribute vec4 a_Color;                " \
		"                                       " \
		"varying vec4 v_Color;                  " \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = vec4(a_Position, 1.0); " \
		" v_Color = a_Color;                    " \
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
		"                                  " \
		"void main()                       " \
		"{                                 " \
		" gl_FragColor = v_Color;          " \
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

	// Ensure the VAO "position" attribute stream gets set as the first position
	// during the link.
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

	/*-----------------------------------------------------------------*/
	//BIND SHADER PROGRAM AND DRAW TRIANGLE
	/*-----------------------------------------------------------------*/

	// Bind the shader to change the uniform, set the uniform and reset state
	glUseProgram(programId);
	glUseProgram(0);


	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(programId);
	glBindVertexArray(vaoId);

	// Draw 3 vertices (a triangle)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);

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

	return 0;
}