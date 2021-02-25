#include "Engine.h"
#define STB_IMAGE_IMPLEMENTATION //Needs to be defined before the include in exacly one compilation unit
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <exception>

void Engine::Initialise()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	window = SDL_CreateWindow("OpenGL Template",
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
}

GLuint Engine::CreateVAO()
{
	GLuint vaoId = 0;

	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &vaoId);

	if (!vaoId)
	{
		throw std::exception();
	}

	return vaoId;
}

GLuint Engine::CreateVBO(const GLfloat * data, int length)
{
	GLuint VBOId = 0;

	//Create a new VBO on the GPU and bind it (Gives us a 'reference' in positionsVboId)
	//1 is number of buffers to initialise - usually just use 1
	glGenBuffers(1, &VBOId);

	if (!VBOId)
	{
		throw std::exception();
	}

	//Now when we operate, we operate on this buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(data) * length, data, GL_STATIC_DRAW);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VBOId;
}

GLuint Engine::CreateExampleVertexShader()
{
	const GLchar* vertexShaderSrc =
		"uniform mat4 u_Projection;           " \
		"uniform mat4 u_Model;                " \
		"attribute vec3 a_Position;             " \
		"attribute vec2 a_TexCoords;             " \
		"                                       " \
		"varying vec2 v_TexCoord;               " \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0); " \
		" v_TexCoord = a_TexCoords;             " \
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
		GLint maxLength = 0; glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog[0]);
		std::cout << &errorLog.at(0) << std::endl;
		throw std::exception();
	}

	return vertexShaderId;
}

GLuint Engine::CreateExampleFragmentShader()
{
	/*const GLchar* fragmentShaderSrc =
		"varying vec4 v_Color;             " \
		"uniform float u_Pulse;            " \
		"                                  " \
		"void main()                       " \
		"{                                 " \
		" gl_FragColor = v_Color * u_Pulse;" \
		"}                                 ";*/

	const GLchar* fragmentShaderSrc =
	"uniform sampler2D u_Texture;      " \
	"varying vec2 v_TexCoord;           " \
	"                                  " \
	"void main()                       " \
	"{                                 " \
	" vec4 tex = texture2D(u_Texture, v_TexCoord);" \
	" gl_FragColor = tex;" \
	"}                                 ";

	// Create a new fragment shader, attach source code, compile it and
	// check for errors.
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderId);
	GLint success = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint maxLength = 0; glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorLog[0]);
		std::cout << &errorLog.at(0) << std::endl; 
		throw std::exception();
	}

	return fragmentShaderId;
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
}

void Engine::Render()
{
}

int Engine::Run()
{
	Initialise();

	const GLfloat positions[] =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	const GLfloat texCoords[] =
	{
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	int width = 0;
	int height = 0;
	unsigned char* data = LoadTextureData("assets/textures/potato.jpg", &width, &height);

	GLint vaoID = CreateVAO();
	GLint coloursVBOID = CreateVBO(texCoords, 12);
	GLint positionsVBOID = CreateVBO(positions, 6);
	GLint textureID = CreateTexture(data, width, height);

	//Bind VBOS to VAOS////////////////////////////////
	glBindVertexArray(vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVBOID);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, coloursVBOID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//////////////////////////////////////////////////

	GLuint vertexShaderID = CreateExampleVertexShader();
	GLuint fragmentShaderID = CreateExampleFragmentShader();

	//////////////////////////////////////////////////
	//Create Program and Attach Shader Objects
	// Create new shader program and attach our shader objects

	GLint success = 0;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glBindAttribLocation(programID, 0, "a_Position");
	glBindAttribLocation(programID, 1, "a_TexCoord");

	// Perform the link and check for failure
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLint maxLength = 0; glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength); 
		glGetShaderInfoLog(programID, maxLength, &maxLength, &errorLog[0]); 
		std::cout << &errorLog.at(0) << std::endl; throw std::exception();
	}


	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	glDetachShader(programID, vertexShaderID);
	glDeleteShader(vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(fragmentShaderID);

	// Store location of uniforms and check if successfully found
	GLint texLoc = glGetUniformLocation(programID, "u_Texture");
	GLint modelLoc = glGetUniformLocation(programID, "u_Model");
	GLint projectionLoc = glGetUniformLocation(programID, "u_Projection");
	if (texLoc == -1 || modelLoc == -1 || projectionLoc == -1)
	{
		throw std::exception();
	}
	/////////////////////////////////////////////////////////////////////////

	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(programID);
	glBindVertexArray(vaoID);

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);

	float delta = 0.0001f;
	float angle = 0;

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
		glViewport(0, 0, width, height);

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
		glUseProgram(programID);

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
		glUseProgram(programID);
		glBindVertexArray(vaoID);

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//ORTHOGRAPHIC DEMO#####################################################
		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f,
			(float)WINDOW_HEIGHT, 0.0f, 1.0f);

			// Prepare model matrix. The scale is important because now our triangle
			// would be the size of a single pixel when mapped to an orthographic
			// projection.
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(100, WINDOW_HEIGHT - 100, 0));
			model = glm::scale(model, glm::vec3(100, 100, 1));

			// Upload the model matrix
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			// Upload the projection matrix
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
				glm::value_ptr(projection));

			// Draw shape as before

			glDrawArrays(GL_TRIANGLES, 0, 3);
		//############################################################################

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
}

Engine::~Engine()
{
	SDL_DestroyWindow(window);
}
