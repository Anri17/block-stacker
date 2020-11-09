#pragma once

#include "Tetrominos.h"
#include "Vector2.h"
#include "Transform.h"

struct Piece
{
	Transform transform;
	TetrominoData piece;
};

Piece create_piece(int x, int y, TetrominoData data)
{
	Vector2 position(x, y);
	Transform transform(position);
	return Piece{ transform, data };
}
