#include <iostream>
#include <SDL.h>

#include "Game.h"
#include "Log.h"

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 720

int main(int argc, char** argv)
{
	const char* title = "Tetris";

	LOG_INFO("Application Start");

	Game* game = new Game(title, WINDOW_WIDTH+ 100, WINDOW_HEIGHT + 100);

	delete game;

	return 0;
}
