#include "Window.h"

Window::Window()
{
	//initlize SDL and GL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
	}

	m_wind = SDL_CreateWindow("Sunset",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		1200, 800,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	glcontext = SDL_GL_CreateContext(m_wind);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW");
}

Window::~Window()
{
	if (m_wind) {
		SDL_DestroyWindow(m_wind);
	}
	if (glcontext)
	{
		SDL_GL_DeleteContext(glcontext);
	}
}