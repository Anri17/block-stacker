#pragma once

#include "Color.h"

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

class Renderer
{
public:
	void DrawSquare(int x, int y, int width, int height, Color color);
	void DrawText(int x, int y, TTF_Font* font, std::string text, Color color);
	void CreateRenderer(SDL_Window* window);
	void Clear();
	void Present();

	SDL_Texture* CreateTexture(int width, int height, Uint32 format, int access, const char* file);
	SDL_Texture* CreateTextureFromSurface(int width, int height, const char* file);

private:
	SDL_Renderer* _renderer;

// Singleton setup
public:
	static Renderer* GetInstance();

private:
	Renderer();
	~Renderer();
	static Renderer* instance;

};
