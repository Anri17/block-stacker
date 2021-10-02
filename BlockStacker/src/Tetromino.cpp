#include "Tetromino.h"

Tetromino::Tetromino()
{
	this->data = 0;
	this->side = 0;
	this->rotation = 0;
	this->color = Color();
}

Tetromino::Tetromino(uint8_t* data, uint8_t side, Color color)
{
	this->data = data;
	this->side = side;
	this->rotation = 0;
	this->color = color;
}

uint8_t Tetromino::GetTetromino(uint8_t row, uint8_t col)
{
	uint8_t width = this->side;

	switch (rotation)
	{
	case 0:
		return this->data[row * width + col];
	case 1:
		return this->data[(width - col - 1) * width + row];
	case 2:
		return this->data[(width - row - 1) * width + (width - col - 1)];
	case 3:
		return this->data[col * width + (width - row - 1)];
	}
	return 0;
}

uint8_t Tetromino::J_data[] = {
		0, 1, 0,
		0, 1, 0,
		1, 1, 0
};

uint8_t Tetromino::L_data[] = {
	0, 1, 0,
	0, 1, 0,
	0, 1, 1
};

uint8_t Tetromino::O_data[] = {
	1, 1,
	1, 1
};

uint8_t Tetromino::T_data[] = {
	0, 1, 0,
	1, 1, 1,
	0, 0, 0
};

uint8_t Tetromino::I_data[] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 0, 1, 0
};

uint8_t Tetromino::S_data[] = {
	0, 1, 1,
	1, 1, 0,
	0, 0, 0
};

uint8_t Tetromino::Z_data[] = {
	1, 1, 0,
	0, 1, 1,
	0, 0, 0
};

Tetromino Tetromino::tetrominoList[] = {
	Tetromino(Tetromino::J_data, 3, Color::BLUE),
	Tetromino(Tetromino::L_data, 3, Color::ORANGE),
	Tetromino(Tetromino::O_data, 2, Color::YELLOW),
	Tetromino(Tetromino::T_data, 3, Color::PURPLE),
	Tetromino(Tetromino::I_data, 4, Color::CYAN),
	Tetromino(Tetromino::S_data, 3, Color::GREEN),
	Tetromino(Tetromino::Z_data, 3, Color::RED)
};
