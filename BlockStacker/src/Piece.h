#pragma once

#include "Tetromino.h"
#include "Transform.h"

struct Piece
{
	Piece(int x, int y, Tetromino data);
	Piece();
	Transform transform;
	Tetromino piece;
};
