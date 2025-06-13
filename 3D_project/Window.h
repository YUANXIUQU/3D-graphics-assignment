#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <iostream>
struct Window
{
public:
	Window();
	~Window();
	SDL_Window* getWin()
	{
		return m_wind;
	}
private:
	SDL_Window* m_wind;
	SDL_GLContext glcontext;
	
};
