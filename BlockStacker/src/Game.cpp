#include "Game.h"
#include "Log.h"

#include "Common/Util.h"
#include "Tetromino.h"

enum Scenes {
	MAIN_GAME = 0,
	MAIN_MENU = 1,
	PAUSE_MENU = 2
};

Game::Game(std::string title, int windows_width, int windows_height) : 
	Application(title, windows_width, windows_height)
{
	game_scene = MAIN_MENU;

	game_over = false;

	current_piece = new Piece();
	ghost_piece = new Piece();

	current_fps = 0;

	TTF_Init();
	std::string basepath = SDL_GetBasePath();
	std::string fontname = "DIOGENES.ttf";
	std::string fontpath = basepath + fontname;
	int fontsize = 20;
	font = TTF_OpenFont(fontpath.c_str(), fontsize);

	if (!font) {
		LOG_ERROR(TTF_GetError());
		// handle error
	}

	testTexture = new Texture(0, 0, 10, 10, "test.png");

	LOG_INFO("Run Game");
	this->Run();
}

Game::~Game()
{
	TTF_CloseFont(font);
	TTF_Quit();

	delete testTexture;
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

			current_piece_copy = *current_piece;

			if (e.type == SDL_KEYDOWN)
			{
				if (game_scene == MAIN_GAME)
				{
					// Main Game Controls
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						game_scene = PAUSE_MENU;
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
				else if (game_scene == MAIN_MENU)
				{
					// Main Menu Controls
					switch (e.key.keysym.sym)
					{
					case SDLK_n:
						ResetGame();
						game_scene = MAIN_GAME;
						break;
					case SDLK_q:
						game_over = true;
						break;
					}
				}
				else if (game_scene == PAUSE_MENU)
				{
					// Pause Menu Controls
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						game_scene = MAIN_GAME;
						break;
					case SDLK_n:
						ResetGame();
						game_scene = MAIN_GAME;
						break;
					case SDLK_q:
						game_over = true;
						break;
					}
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
			SDL_Delay(frameDelay - (float)frameTime);
		}

#ifdef _DEBUG
		current_fps = (int)maxFPS - frameTime;
#endif
	}
}

void Game::Update()
{
	if (game_scene == MAIN_GAME)
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
			if (CheckCollision(*current_piece)) game_scene = MAIN_MENU;		// since the new piece collides with the board, it's game over and the game ends
		}
	}
	// check if the piece is colliding with the bottom
	// add that piece to an array of lines that are to be calculated.
	// Also check for collision with that array of lines
}

void Game::Render()
{
	_renderer->Clear();
		
	if (game_scene == MAIN_GAME)
	{
		// draw ghost piece
		DrawTetromino(ghost_piece->transform.position.x, ghost_piece->transform.position.y, 30, &ghost_piece->piece, ghost_piece->piece.GetColor());
		// draw real piece
		DrawTetromino(current_piece->transform.position.x, current_piece->transform.position.y, 30, &current_piece->piece, current_piece->piece.GetColor());
		// draw board
		DrawBoard(30, board);
	}
	else if (game_scene == MAIN_MENU)
	{
 		_renderer->DrawSquare(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color::GREY);

		_renderer->DrawText((WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 160, font, "N - New Game", Color::BLACK);
		_renderer->DrawText((WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 120, font, "Q - Quit", Color::BLACK);
	}
	else if (game_scene == PAUSE_MENU)
	{
		_renderer->DrawSquare(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color::GREY);

		_renderer->DrawText((WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 160, font, "ESQ - Resume", Color::BLACK);
		_renderer->DrawText((WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 120, font, "N - New Game", Color::BLACK);
		_renderer->DrawText((WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 80, font, "Q - Quit", Color::BLACK);
	}


#ifdef _DEBUG
	// draw FPS
	DrawFPS();
#endif

	_renderer->Present();
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
	uint8_t side = tetromino->GetSide();

	for (uint8_t row = 0; row < side; row++)
	{
		for (uint8_t col = 0; col < side; col++)
		{
			uint8_t currentTetrominoBlock = tetromino->GetTetromino(row, col);

			if (currentTetrominoBlock == 0) continue;

			int xPos = x * width + col * width;
			int yPos = y * width + row * width;

			_renderer->DrawSquare(xPos, yPos, width, width, color);
		}
	}
}

void Game::SetRandomPiece(Piece* piece)
{
	int x = board_width / 2 - 1;
	int y = 1;

	int randomIndex = Util::Random(7);

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

			_renderer->DrawSquare(xPos, yPos, width, width, color);
		}
	}
}

void Game::DrawFPS()
{
	/* Display FPS */
	std::string fps_text = "FPS: " + std::to_string(current_fps);
	
	_renderer->DrawText(0, 0, font, fps_text, Color::WHITE);

	_renderer->DrawText(0, 20, font, "Honestly, idk if I'm", Color::WHITE);
	_renderer->DrawText(0, 40, font, "calculating the fps correctly.", Color::WHITE);
	_renderer->DrawText(0, 60, font, "Don\'t take it seriously.", Color::WHITE);
}

void Game::SetGhostPiece(Piece* ghost_piece, Piece& copy_piece)
{
	// get the board and check if the piece collides. if it does, set it to the last point it did not colide
	int x = copy_piece.transform.position.x;
	int y = copy_piece.transform.position.y;
	Tetromino data = copy_piece.piece;

	// change the color of the piece
	data.GetColor() = Color::GREY;

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
	uint8_t side = piece.piece.GetSide();
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
	for (int row = 0; row < tetromino.GetSide(); row++)
	{
		for (int col = 0; col < tetromino.GetSide(); col++)
		{
			uint8_t block = tetromino.GetTetromino(row, col);

			if (block == 1)
			{
 				board[(row + piece.transform.position.y - 2) * board_width + (col + piece.transform.position.x)] = block;
			}
		}
	}
}

void Game::ResetGame()
{
	// clear all lines
	for (int row = 0; row < BOARD_HEIGHT; row++)
	{
		for (int col = 0; col < BOARD_WIDTH; col++)
		{
			board[row * board_width + col] = 0;
		}
	}

	// reset player piece position
	SetRandomPiece(current_piece);
	SetRandomPiece(&current_piece_copy);
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
