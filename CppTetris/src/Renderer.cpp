#include "Renderer.h"

#include <SDL_image.h>
#include "Log.h"

Renderer* Renderer::GetInstance()
{
    if (!instance)
        instance = new Renderer();
    return instance;
}

Renderer::Renderer()
	: _renderer(nullptr)
{
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		LOG_ERROR("SDL_image could not initialize! SDL_image Error: " << IMG_GetError());
	}
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(_renderer);
	IMG_Quit();

    delete instance;
}

Renderer* Renderer::instance = 0; // initialize instance

void Renderer::DrawSquare(int x, int y, int width, int height, Color color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			SDL_RenderDrawPoint(_renderer, x + col, y + row);
		}
	}
}

void Renderer::DrawText(int x, int y, TTF_Font* font, std::string text, Color color)
{
	SDL_Color sdl_color = { color.r, color.g, color.b };

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), sdl_color);

	SDL_Texture* fps_texture = SDL_CreateTextureFromSurface(_renderer, textSurface);

	SDL_Rect texture_rect;

	texture_rect.x = x;
	texture_rect.y = y;
	texture_rect.w = textSurface->w;
	texture_rect.h = textSurface->h;

	SDL_FreeSurface(textSurface);

	SDL_RenderCopy(_renderer, fps_texture, NULL, &texture_rect);
}

void Renderer::CreateRenderer(SDL_Window* window)
{
	_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Renderer::Clear()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(_renderer);
}

void Renderer::Present()
{
	SDL_RenderPresent(_renderer);
}

SDL_Texture* Renderer::CreateTexture(int width, int height, Uint32 format, int access, const char* file)
{
	return SDL_CreateTexture(_renderer, format, access, width, height);
}

SDL_Texture* Renderer::CreateTextureFromSurface(int width, int height, const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

	SDL_FreeSurface(surface);

	return texture;
}
