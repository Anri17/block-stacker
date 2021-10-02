#pragma once

#include "Color.h"
#include "Vector2.h"

class TetrominoBlock
{
public:
	TetrominoBlock(Color color, int size);
	~TetrominoBlock();
private:
	Color m_Color;
	Vector2 m_Position;
	int size;
};
