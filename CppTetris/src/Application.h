#pragma once

#include <string>
#include <SDL.h>
#include "Renderer.h"
#include "Log.h"

class Application
{
public:
	Application(std::string title, int width, int height);
	~Application();
protected:
	int _windowWidth;
	int _windowHeight;
	std::string _windowTitle;
	SDL_Window* _window;
	Renderer* _renderer;
};
