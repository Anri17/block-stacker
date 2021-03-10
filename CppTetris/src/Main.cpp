#include <iostream>
#include <SDL.h>

#include "Game.h"
#include "Log.h"

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 720

int main(int argc, char** argv)
{
	std::string title = "Tetris";

	LOG_INFO("Application Start");

	Game* game = new Game(title, WINDOW_WIDTH, WINDOW_HEIGHT);

	delete game;

	return 0;
}
