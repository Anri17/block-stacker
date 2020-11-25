#pragma once

#include <SDL.h>
#include <chrono>
#include "Piece.h"
#include "Tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22

class Game
{
public:
	Game(const char* title, int windows_width, int windows_height);
	~Game();
	void Run();
private:
	// methods
	void Update();
	void Render();
	// game loop and SDL variables
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event e;
	bool game_over;
	// game variables
	Piece* current_piece;
	Piece* ghost_piece;
	float howLongToWait = 60;		// This is how many frames to wait until the piece is to be dropped again
	float howLongHasPassed = 0;		// This is how many frames it has passed since the piece was dropped once
	int board_width = 10;
	int board_height = 22;
	uint8_t board[BOARD_WIDTH * BOARD_HEIGHT];			// because board_width * board_height won't work. TODO: fix this
	// game methods
	void DropPiece(Piece& piece);
	void SoftDrop(Piece& piece);
	void DrawTetromino(int x, int y, uint8_t width, Tetromino* tetromino, Color color);
	void DrawBlock(int x, int y, uint8_t width, uint8_t height, Color color);
	void SetRandomPiece(Piece* piece);
	void DrawBoard(uint8_t width, const uint8_t* board);
	void ClearLine(uint8_t* board);
	void SetGhostPiece(Piece* ghost_piece, Piece& copy_piece);
	bool CheckCollision(Piece piece);
	uint8_t GetMatrix(uint8_t* data, uint8_t width, uint8_t row, uint8_t col);
	bool IsFullLine(uint8_t* board, uint8_t row);
	void MoveLineDown(uint8_t* board, uint8_t row_limit);
	void AddPieceToBoard(Piece& piece);
	void ClearLines();
};
