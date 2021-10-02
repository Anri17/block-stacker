#include "Piece.h"

Piece::Piece(int x, int y, Tetromino data)
{
	this->transform = Transform(Vector2(x, y));
	piece = data;
}

Piece::Piece()
{
	this->transform = Transform();
}
