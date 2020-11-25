#pragma once
#include <cstdint>

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	Color();
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	// default values
	static const Color WHITE;
	static const Color GREY;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color ORANGE;
	static const Color PINK;
	static const Color PURPLE;
};
