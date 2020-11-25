#include "Game.h"
#include "Log.h"

Game::Game(const char* title, int windows_width, int windows_height)
{
	LOG_INFO("Initializing SDL");
	SDL_Init(SDL_INIT_EVERYTHING);

	game_over = false;
	

	LOG_INFO("Creating Window");
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windows_width, windows_height, SDL_WINDOW_OPENGL);
	LOG_INFO("Creating Renderer");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	current_piece = new Piece();
	ghost_piece = new Piece();

	LOG_INFO("Run Game");
	this->Run();
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Game::Run()
{
	float maxFPS = 120.0f;
	float frameDelay = 1000.0f / maxFPS;

	Uint64 frameStart;
	Uint64 frameTime;

	SetRandomPiece(current_piece);

	LOG_INFO("Start Loop")
	while (!game_over)
	{
		frameStart = SDL_GetTicks();	// start of frame

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				game_over = true;
			}

			Piece current_piece_copy = *current_piece;

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
					break;
				case SDLK_RIGHT:
					current_piece_copy.transform.position.x++;
					break;
				case SDLK_DOWN:
					DropPiece(current_piece_copy);
					howLongHasPassed = 0;
					break;
				case SDLK_SPACE:
					int lowestY = ghost_piece->transform.position.y;
					current_piece_copy.transform.position.y = lowestY;
					howLongHasPassed = howLongToWait;
					break;
				}
			}

			if (!CheckCollision(current_piece_copy))
			{
				*current_piece = current_piece_copy;
			}
		}

		Update();
		Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)	// delay frame time
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
}

void Game::Update()
{
	SetGhostPiece(ghost_piece, *current_piece);

	Piece current_piece_copy = *current_piece;
	SoftDrop(current_piece_copy);

	if (!CheckCollision(current_piece_copy))
	{
		*current_piece = current_piece_copy;
	}
	else
	{
		AddPieceToBoard(*current_piece);

		// clear all existing lines
		ClearLines();

		SetRandomPiece(current_piece);
		SetGhostPiece(ghost_piece, *current_piece);
		if (CheckCollision(*current_piece)) game_over = true;		// since the new piece collides with the board, it's game over and the game ends
	}

	// check if the piece is colliding with the bottom
	// add that piece to an array of lines that are to be calculated.
	// Also check for collision with that array of lines
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	// draw ghost piece
	DrawTetromino(ghost_piece->transform.position.x, ghost_piece->transform.position.y, 30, &ghost_piece->piece, ghost_piece->piece.color);
	// draw real piece
	DrawTetromino(current_piece->transform.position.x, current_piece->transform.position.y, 30, &current_piece->piece, current_piece->piece.color);
	// draw board
	DrawBoard(30, board);

	SDL_RenderPresent(renderer);
}

void Game::DropPiece(Piece& piece)
{
	piece.transform.position.y++;
}

void Game::SoftDrop(Piece& piece)
{
	if (howLongHasPassed > howLongToWait)
	{
		DropPiece(piece);
		howLongHasPassed = 0;
	}

	howLongHasPassed++;
}

void Game::DrawTetromino(int x, int y, uint8_t width, Tetromino* tetromino, Color color)
{
	uint8_t side = tetromino->side;

	for (uint8_t row = 0; row < side; row++)
	{
		for (uint8_t col = 0; col < side; col++)
		{
			uint8_t currentTetrominoBlock = tetromino->GetTetromino(row, col);

			if (currentTetrominoBlock == 0) continue;

			int xPos = x * width + col * width;
			int yPos = y * width + row * width;

			DrawBlock(xPos, yPos, width, width, color);
		}
	}
}

void Game::DrawBlock(int x, int y, uint8_t width, uint8_t height, Color color)
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

void Game::SetRandomPiece(Piece* piece)
{
	int x = board_width / 2 - 1;
	int y = 1;

	srand(time(NULL));

	int randomIndex = rand() % 7;

	Tetromino tetr = Tetromino::tetrominoList[randomIndex];

	Piece newPiece(x, y, tetr);

	*piece = newPiece;
}

void Game::DrawBoard(uint8_t width, const uint8_t* board)
{
	for (uint8_t row = 0; row < board_height; row++)
	{
		for (uint8_t col = 0; col < board_width; col++)
		{
			uint8_t currentTetrominoBlock = board[row * board_width + col];

			if (currentTetrominoBlock == 0) continue;		// TODO: get rotation

			int xPos = col * width;
			int yPos = row * width + 2 * width;

			Color color(255, 255, 255, 255);

			DrawBlock(xPos, yPos, width, width, color);
		}
	}
}

void Game::SetGhostPiece(Piece* ghost_piece, Piece& copy_piece)
{
	// get the board and check if the piece collides. if it does, set it to the last point it did not colide
	int x = copy_piece.transform.position.x;
	int y = copy_piece.transform.position.y;
	Tetromino data = copy_piece.piece;

	// change the color of the piece
	data.color = Color::GREY;

	Piece gost_piece_copy(x, y, data);

	Piece copy = gost_piece_copy;

	while (!CheckCollision(copy))
	{
		gost_piece_copy = copy;
		copy.transform.position.y++;
	}

	*ghost_piece = gost_piece_copy;
}

bool Game::CheckCollision(Piece piece)
{
	uint8_t side = piece.piece.side;
	Tetromino tetromino = piece.piece;

	for (uint8_t row = 0; row < side; row++)
	{
		for (uint8_t col = 0; col < side; col++)
		{
			uint8_t currentTetrominoBlock = tetromino.GetTetromino(row, col);

			if (currentTetrominoBlock == 0) continue;

			if (currentTetrominoBlock == 1)		// check only the populated part of the array data
			{
				// check collision with sides
				int minX = 0 - col;
				int maxX = board_width - col - 1;
				if (piece.transform.position.x > maxX) return true;
				else if (piece.transform.position.x < minX) return true;

				// check collision with bottom
				int maxY = board_height - row + 1;
				if (piece.transform.position.y > maxY) return true;

				// check collision with board lines
				if (GetMatrix(board, board_width, row + piece.transform.position.y - 2, col + piece.transform.position.x) == 1) return true;
			}
		}
	}
	return false;
}

uint8_t Game::GetMatrix(uint8_t* data, uint8_t width, uint8_t row, uint8_t col)
{
	return data[row * width + col];
}

bool Game::IsFullLine(uint8_t* board, uint8_t row)
{
	bool is_full = true;
	for (int col = 0; col < board_width; col++)
	{
		if (board[row * board_width + col] == 1)
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

void Game::ClearLine(uint8_t* board)
{
	for (int row = 0; row < board_height; row++)
	{
		if (IsFullLine(board, row))
		{
			MoveLineDown(board, row);
		}
	}
	// remove lines at a certain row value
	// move everything above that row up to that line
}

void Game::MoveLineDown(uint8_t* board, uint8_t row_limit)
{
	for (int col = 0; col < board_width; col++)
	{
		board[row_limit * board_width + col] = 0;
	}

	for (int row = row_limit; row > 0; row--)
	{
		for (int col = 0; col < board_width; col++)
		{
			board[(row)*board_width + col] = board[(row - 1) * board_width + col];
		}
	}
}

void Game::AddPieceToBoard(Piece& piece)
{
	Tetromino tetromino = piece.piece;
	for (int row = 0; row < tetromino.side; row++)
	{
		for (int col = 0; col < tetromino.side; col++)
		{
			uint8_t block = tetromino.GetTetromino(row, col);

			if (block == 1)
			{
 				board[(row + piece.transform.position.y - 2) * board_width + (col + piece.transform.position.x)] = block;
			}
		}
	}
}

void Game::ClearLines()
{
	for (int row = 0; row < board_height; row++)
	{
		if (IsFullLine(board, row))
		{
			MoveLineDown(board, row);
		}
	}
	// remove lines at a certain row value
	// move everything above that row up to that line
}

