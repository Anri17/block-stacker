#pragma once

#include "Color.h"

#include <SDL.h>

class Renderer
{
public:
	void DrawSquare(int x, int y, int width, int height, Color color);
	void CreateRenderer(SDL_Window* window);
	void Clear();
	void Present();
private:
	SDL_Renderer* renderer;

// Singleton setup
public:
	static Renderer* GetInstance();
private:
	Renderer();
	~Renderer();
	static Renderer* instance;

};
