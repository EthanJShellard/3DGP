#include "Engine.h"

void Engine::Initialise()
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

GLuint Engine::CreateVBO(const void * data)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VBOId;
}
