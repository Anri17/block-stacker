#include <SDL.h>

#include "Piece.h"
#include <chrono>
#include <iostream>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 720

#define LOG(str) std::cout << str << std::endl;

uint8_t board[BOARD_WIDTH * BOARD_HEIGHT];

Piece current_piece;

Piece ghost_piece;

Piece create_random_piece()
{
	int x = BOARD_WIDTH / 2 - 1;
	int y = 1;

	srand(time(NULL));

	int randomIndex = rand() % 7;

	Piece piece = create_piece(x, y, tetrominos[randomIndex]);

	return piece;
}

uint8_t get_matrix(uint8_t* data, uint8_t width, uint8_t row, uint8_t col)
{
	return data[row * width + col];
}

bool check_collision(Piece* piece)			// check for general collision cases
{
	uint8_t side = piece->piece.side;
	TetrominoData* tetromino = &piece->piece;

	// todo: check collision only for the right elemennts of the array
	for (uint8_t row = 0; row < side; row++)
	{
		for (uint8_t col = 0; col < side; col++)
		{
			uint8_t currentTetrominoBlock = get_tetromino(tetromino, row, col, tetromino->rotation);

			if (currentTetrominoBlock == 0) continue;

			if (currentTetrominoBlock == 1)		// check only the populated part of the array data
			{
				// check collision with sides
				int minX = 0 - col;
				int maxX = BOARD_WIDTH - col - 1;
				if (piece->transform.position.x > maxX) return true;
				else if (piece->transform.position.x < minX) return true;

				// check collision with bottom
				int maxY = BOARD_HEIGHT - row +1;
				if (piece->transform.position.y > maxY) return true;

				// check collision with board lines
				if (get_matrix(board, BOARD_WIDTH, row + piece->transform.position.y - 2, col + piece->transform.position.x) == 1) return true;
			}
		}
	}
	return false;
}

void draw_block(SDL_Renderer* renderer, int x, int y, uint8_t width, uint8_t height, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			SDL_RenderDrawPoint(renderer, x + col, y + row);
		}
	}
}

void draw_tetromino(SDL_Renderer* renderer, int x, int y, uint8_t width, TetrominoData* tetromino, Color color)
{
	uint8_t side = tetromino->side;

	for (uint8_t row = 0; row < side; row++)
	{
		for (uint8_t col = 0; col < side; col++)
		{
			uint8_t currentTetrominoBlock = get_tetromino(tetromino, row, col, tetromino->rotation);

			if (currentTetrominoBlock == 0) continue;

			int xPos = x * width + col * width;
			int yPos = y * width + row * width;

			draw_block(renderer, xPos, yPos, width, width, color);
		}
	}
}

void draw_board(SDL_Renderer* renderer, uint8_t width, const uint8_t* board)
{
	for (uint8_t row = 0; row < BOARD_HEIGHT; row++)
	{
		for (uint8_t col = 0; col < BOARD_WIDTH; col++)
		{
			uint8_t currentTetrominoBlock = board[row * BOARD_WIDTH + col];

			if (currentTetrominoBlock == 0) continue;		// TODO: get rotation

			int xPos = col * width;
			int yPos = row * width + 2 * width;

			Color color(255, 255, 255, 255);

			draw_block(renderer, xPos, yPos, width, width, color);
		}
	}
}

void print_board(uint8_t* board)
{
	std::cout << std::endl;
	for (uint8_t row = 0; row < BOARD_HEIGHT; row++)
	{
		for (uint8_t col = 0; col < BOARD_WIDTH; col++)
		{
			int index = row * BOARD_WIDTH + col;
			std::cout << (int)board[index];
		}
		std::cout << std::endl;
	}
}

void render_game(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	// draw ghost piece
	draw_tetromino(renderer, ghost_piece.transform.position.x, ghost_piece.transform.position.y, 30, &ghost_piece.piece, *ghost_piece.piece.color);
	// draw real piece
	draw_tetromino(renderer, current_piece.transform.position.x, current_piece.transform.position.y, 30, &current_piece.piece, tetromino_color);
	// draw board
	draw_board(renderer, 30, board);

	SDL_RenderPresent(renderer);
}

bool is_full_line(uint8_t* board, uint8_t row)
{
	bool is_full;
	for (int col = 0; col < BOARD_WIDTH; col++)
	{
		if (board[row * BOARD_WIDTH + col] == 1)
		{
			is_full = true;
		}
		else
		{
			is_full = false;
			break;
		}
	}
	return is_full;
}

void move_line_down(uint8_t* board, uint8_t row_limit)
{
	for (int col = 0; col < BOARD_WIDTH; col++)
	{
		board[row_limit * BOARD_WIDTH + col] = 0;
	}

	for (int row = row_limit; row > 0; row--)
	{
		for (int col = 0; col < BOARD_WIDTH; col++)
		{
			board[(row) * BOARD_WIDTH + col] = board[(row-1) * BOARD_WIDTH + col];
		}
	}
}

Piece* set_ghost_piece(Piece* piece)
{
	// get the board and check if the piece collides. if it does, set it to the last point it did not colide
	int x = piece->transform.position.x;
	int y = piece->transform.position.y;
	TetrominoData data = piece->piece;
	
	// change the color of the piece
	Color shadow_color(128, 128, 128, 128);
	data.color = &shadow_color;

	Piece gost_piece_copy = create_piece(x, y, data);

	Piece copy = gost_piece_copy;

	while (!check_collision(&copy))
	{
		gost_piece_copy = copy;
		copy.transform.position.y++;
	}

	return &gost_piece_copy;
}

void clear_lines(uint8_t* board)
{
	for (int row = 0; row < BOARD_HEIGHT; row++)
	{
		if (is_full_line(board, row))
		{
			move_line_down(board, row);
		}
	}
	// remove lines at a certain row value
	// move everything above that row up to that line
}

void drop_piece(Piece* piece)
{
	piece->transform.position.y++;
	LOG("piece x: " << current_piece.transform.position.x);
	LOG("piece y: " << current_piece.transform.position.y);
	print_board(board);
}

float howLongToWait = 60;		// This is how many frames to wait until the piece is to be dropped again
float howLongHasPassed = 0;		// This is how many frames it has passed since the piece was dropped once

void soft_drop(Piece* piece)
{
	if (howLongHasPassed > howLongToWait)
	{
		drop_piece(piece);
		howLongHasPassed = 0;
	}

	howLongHasPassed++;
}

void add_piece_to_board(uint8_t* board, Piece piece)
{
	TetrominoData tetromino = piece.piece;
	for (int row = 0; row < tetromino.side; row++)
	{
		for (int col = 0; col < tetromino.side; col++)
		{
			uint8_t block = get_tetromino(&tetromino, row, col, tetromino.rotation);

			if (block == 1)
			{
				board[(row + piece.transform.position.y-2) * BOARD_WIDTH + (col + piece.transform.position.x)] = block;
			}
		}
	}
}

bool game_over = false;

void update_game()
{

	ghost_piece = *set_ghost_piece(&current_piece);

	Piece current_piece_copy = current_piece;
	soft_drop(&current_piece_copy);
	if (!check_collision(&current_piece_copy))
	{
		current_piece = current_piece_copy;
	}
	else
	{
		add_piece_to_board(board, current_piece);

		// clear all existing lines
		clear_lines(board);

		current_piece = create_random_piece();
		if (check_collision(&current_piece)) game_over = true;		// since the new piece collides with the board, it's game over and the game ends
	}

	// check if the piece is colliding with the bottom
	// add that piece to an array of lines that are to be calculated.
	// Also check for collision with that array of lines
}

float maxFPS = 120.0;
float frameDelay = 1000.0 / maxFPS;

Uint64 frameStart;
int frameTime;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	current_piece = create_random_piece();

	while (!game_over)
	{
		frameStart = SDL_GetTicks();	// start of frame

		SDL_Event e;

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				game_over = true;
			}

			Piece current_piece_copy = current_piece;

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					game_over = true;
					break;
				case SDLK_UP:
					current_piece_copy.piece.rotation = (current_piece_copy.piece.rotation + 1) % 4;
					howLongHasPassed = 0;
					break;
				case SDLK_LEFT:
					current_piece_copy.transform.position.x--;
					howLongHasPassed = 0;
					break;
				case SDLK_RIGHT:
					current_piece_copy.transform.position.x++;
					howLongHasPassed = 0;
					break;
				case SDLK_DOWN:
					drop_piece(&current_piece_copy);
					howLongHasPassed = 0;
					break;
				case SDLK_SPACE:
					int lowestY = ghost_piece.transform.position.y;
					current_piece_copy.transform.position.y = lowestY;
					howLongHasPassed = howLongToWait;
					break;
				}
			}

			if (!check_collision(&current_piece_copy))
			{
				current_piece = current_piece_copy;
			}
		}

		update_game();
		render_game(renderer);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)	// delay frame time
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
