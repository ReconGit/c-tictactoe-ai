#include <stdbool.h>

// Represents the state of the game.
enum Winner {
    NOT_FINISHED, X_WIN, O_WIN, DRAW
};

// Represents a coordinate on the board.
struct Move {
    int x;
    int y;
};

// Checks game state and returns winner.
enum Winner check_winner(char board[3][3]);

// Checks if a move is valid.
bool is_move_valid(char board[3][3], struct Move move);
