#include "Application.h"

Application::Application(std::string title, int width, int height)
{
	_windowTitle = title;
	_windowWidth = width;
	_windowHeight = height;

	LOG_INFO("Initializing SDL");
	SDL_Init(SDL_INIT_EVERYTHING);

	LOG_INFO("Creating Window");
	_window = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL);

	LOG_INFO("Creating Renderer");
	_renderer = Renderer::GetInstance();
	_renderer->CreateRenderer(_window);
}

Application::~Application()
{
	SDL_DestroyWindow(_window);

	SDL_ShowCursor(SDL_DISABLE);

	SDL_Quit();
}
