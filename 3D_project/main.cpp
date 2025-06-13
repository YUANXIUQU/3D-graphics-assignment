#define SDL_MAIN_HANDLED 

#include "Game.h"
//
//#define WINDOW_WIDTH 1200
//#define WINDOW_HEIGHT 800

#undef main

int main()
{
	Game game;
	srand(time(0));
	game.run();
	return 0;
}