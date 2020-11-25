#pragma once
#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>

#include "Color.h"

struct Tetromino
{
	Tetromino();
	Tetromino(uint8_t* data, uint8_t side, Color color);
	uint8_t* data;
	uint8_t side;
	uint8_t rotation;
	Color color;
	static Tetromino tetrominoList[7];
	uint8_t GetTetromino(uint8_t row, uint8_t col);
	static uint8_t J_data[];
	static uint8_t L_data[];
	static uint8_t O_data[];
	static uint8_t T_data[];
	static uint8_t I_data[];
	static uint8_t S_data[];
	static uint8_t Z_data[];
};

#endif // TETROMINO_H
