#pragma once

#include "Color.h"

class Tetromino
{
public:
	Tetromino();
	Tetromino(uint8_t* data, uint8_t side, Color color);
	uint8_t rotation;
	static Tetromino tetrominoList[7];
	uint8_t GetTetromino(uint8_t row, uint8_t col);
	static uint8_t J_data[];
	static uint8_t L_data[];
	static uint8_t O_data[];
	static uint8_t T_data[];
	static uint8_t I_data[];
	static uint8_t S_data[];
	static uint8_t Z_data[];
	inline uint8_t GetSide() const { return side; }
	inline Color GetColor() const { return color; }
	inline void SetColor(const Color color) { this->color = color; }
private:
	uint8_t* data;
	uint8_t side;
	Color color;
};
