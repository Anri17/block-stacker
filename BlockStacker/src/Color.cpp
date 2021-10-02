#include "Color.h"

Color::Color()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

const Color Color::BLACK(0, 0, 0, 255);
const Color Color::WHITE(255, 255, 255, 255);
const Color Color::GREY(128, 128, 128, 128);

const Color Color::RED(255, 0, 0, 255);
const Color Color::GREEN(0, 255, 0, 255);
const Color Color::BLUE(0, 0, 255, 255);

const Color Color::PINK(255, 0, 255, 255);
const Color Color::PURPLE(128, 0, 255, 255);
const Color Color::CYAN(0, 255, 255, 255);
const Color Color::YELLOW(255, 255, 0, 255);
const Color Color::ORANGE(255, 128, 0, 255);
