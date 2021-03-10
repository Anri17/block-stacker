#include "Renderer.h"

Renderer* Renderer::GetInstance()
{
    if (!instance)
        instance = new Renderer();
    return instance;
}

Renderer::Renderer()
	: renderer(nullptr)
{
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
    delete instance;
}

Renderer* Renderer::instance = 0; // initialize instance

void Renderer::DrawSquare(int x, int y, int width, int height, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			SDL_RenderDrawPoint(renderer, x + col, y + row);
		}
	}
}

void Renderer::CreateRenderer(SDL_Window* window)
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Renderer::Clear()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void Renderer::Present()
{
	SDL_RenderPresent(renderer);
}
