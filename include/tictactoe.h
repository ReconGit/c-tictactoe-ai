#include <stdbool.h>

// Represents the state of the game.
enum Winner {
    PLAYING, X_WIN, O_WIN, DRAW
};

// Represents a coordinate on the board.
struct Move {
    int x;  // column
    int y;  // row
};

// Checks game state and returns winner.
enum Winner check_winner(char board[3][3]);

// Checks if a move is valid.
bool is_move_valid(char board[3][3], struct Move move);

// Fills the moves array with valid moves and returns their count
int get_valid_moves(char board[3][3], struct Move moves[9]);
