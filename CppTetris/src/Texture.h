#pragma once

#include <SDL.h>
#include "Transform.h"

class Texture
{
public:
	Texture(int x, int y, int width, int height, const char* file);
	~Texture();

	inline Transform GetTransform() const { return _transform; }
	inline void SetTransform(Transform transform) { _transform = transform; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
private:
	SDL_Texture* _texture;
	Transform _transform;
	int _width;
	int _height;
};

