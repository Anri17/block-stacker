#pragma once

#include <iostream>
#include "Color.h"

struct TetrominoData
{
	uint8_t* data;
	uint8_t side;
	uint8_t rotation;
	Color* color;
};

TetrominoData create_tetrominoData(std::uint8_t* data, std::uint8_t size, Color color)
{
	return TetrominoData{ data, size, 0, &color};
}

uint8_t tetromino_J[] = {
	0, 1, 0,
	0, 1, 0,
	1, 1, 0
};

uint8_t tetromino_L[] = {
	0, 1, 0,
	0, 1, 0,
	0, 1, 1
};

uint8_t tetromino_O[] = {
	1, 1,
	1, 1
};

uint8_t tetromino_T[] = {
	0, 1, 0,
	1, 1, 1,
	0, 0, 0
};

uint8_t tetromino_I[] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0
};

uint8_t tetromino_S[] = {
	0, 1, 1,
	1, 1, 0,
	0, 0, 0
};

uint8_t tetromino_Z[] = {
	1, 1, 0,
	0, 1, 1,
	0, 0, 0
};

Color tetromino_color(100, 100, 100, 255);

TetrominoData tetrominos[] = {
	create_tetrominoData(tetromino_J, 3, tetromino_color),
	create_tetrominoData(tetromino_L, 3, tetromino_color),
	create_tetrominoData(tetromino_O, 2, tetromino_color),
	create_tetrominoData(tetromino_T, 3, tetromino_color),
	create_tetrominoData(tetromino_I, 4, tetromino_color),
	create_tetrominoData(tetromino_S, 3, tetromino_color),
	create_tetrominoData(tetromino_Z, 3, tetromino_color),
};

void print_tetromino_data(TetrominoData* tetrominos, uint8_t array_size)
{
	for (uint8_t i = 0; i < array_size; ++i)
	{
		std::cout << "size: " << (int)tetrominos[i].side << "\t\t" << "data: " << " ";
		for (uint8_t j = 0; j < tetrominos[i].side * tetrominos[i].side; j++)
		{
			std::cout << (int)tetrominos[i].data[j] << " ";
		}
		std::cout << std::endl;
	}
}

uint8_t get_tetromino(TetrominoData* tetromino, uint8_t row, uint8_t col, uint8_t rotation)
{
	uint8_t width = tetromino->side;

	switch (rotation)
	{
	case 0:
		return tetromino->data[row * width + col];
	case 1:
		return tetromino->data[(width - col - 1) * width + row];
	case 2:
		return tetromino->data[(width - row - 1) * width + (width - col - 1)];
	case 3:
		return tetromino->data[col * width + (width - row - 1)];
	}
	return 0;
}
