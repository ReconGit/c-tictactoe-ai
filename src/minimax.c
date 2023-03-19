#include "../include/minimax.h"
#include "../include/tictactoe.h"

#define DEPTH 9
#define INT_MIN -2147483648
#define INT_MAX 2147483647

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static int minimax(char board[3][3], char ai_player, int depth, bool ai_turn, int alpha, int beta);

struct Move find_best_move(char board[3][3], char ai_player)
{
    int best_value = INT_MIN;
    struct Move best_move = {-1, -1};
    for (int y = 0; y < 3; y++) {  // branch each valid move
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == ' ') {
                board[y][x] = ai_player;  // try move
                int move_value = minimax(board, ai_player, DEPTH - 1, false, INT_MIN, INT_MAX);
                board[y][x] = ' ';  // undo move

                if (move_value > best_value) {
                    best_move.y = y;
                    best_move.x = x;
                    best_value = move_value;
                }
            }
        }
    }
    return best_move;
}

static int minimax(char board[3][3], char ai_player, int depth, bool ai_turn, int alpha, int beta)
{
    // win as soon as possible, lose as late as possible using depth
    enum Winner winner = check_winner(board);
    if (winner == X_WIN) {
        return (ai_player == 'X') ? 10 + depth : -10 - depth;
    } else if (winner == O_WIN) {
        return (ai_player == 'O') ? 10 + depth : -10 - depth;
    } else if (winner == DRAW || depth == 0) {
        return 0;
    }

    int value;
    if (ai_turn) {  // maximize ai player move
        value = INT_MIN;
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (board[y][x] == ' ') {
                    board[y][x] = ai_player;
                    value = MAX(value, minimax(board, ai_player, depth - 1, false, alpha, beta));
                    board[y][x] = ' ';

                    alpha = MAX(alpha, value);
                    if (alpha >= beta) {
                        return value;
                    }
                }
            }
        }
    } else {  // minimize opponent move
        char opponent = (ai_player == 'X') ? 'O' : 'X';
        value = INT_MAX;
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (board[y][x] == ' ') {
                    board[y][x] = opponent;
                    value = MIN(value, minimax(board, ai_player, depth - 1, true, alpha, beta));
                    board[y][x] = ' ';

                    beta = MIN(beta, value);
                    if (alpha >= beta) {
                        return value;
                    }
                }
            }
        }
    }
    return value;
}
