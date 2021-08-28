#include "Texture.h"
#include "Renderer.h"

Texture::Texture(int x, int y, int width, int height, const char* file)
{
	_texture = Renderer::GetInstance()->CreateTextureFromSurface(width, height, file);
	_width = width;
	_height = height;

	Vector2 position(x, y);

	_transform.position = position;
}

Texture::~Texture()
{
	SDL_DestroyTexture(_texture);
}
