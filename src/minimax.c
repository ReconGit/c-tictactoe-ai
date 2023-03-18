#include "../include/minimax.h"
#include "../include/tictactoe.h"

static int min(int a, int b) { return (a < b) ? a : b; }
static int max(int a, int b) { return (a > b) ? a : b; }

static int minimax(char board[3][3], char ai_player, bool ai_turn, int alpha, int beta);

struct Move find_best_move(char board[3][3], char ai_player)
{
    int best_value = -9999;
    struct Move best_move;
    for (int y = 0; y < 3;y++) {  // branch each possible move
        for (int x = 0; x < 3;x++) {
            if (board[x][y] == ' ') {
                board[x][y] = ai_player;
                int move_value = minimax(board, ai_player, false, -9999, 9999);
                board[x][y] = ' ';

                if (move_value > best_value) {
                    best_move.x = x;
                    best_move.y = y;
                    best_value = move_value;
                }
            }
        }
    }
    return best_move;
}

static int minimax(char board[3][3], char ai_player, bool ai_turn, int alpha, int beta)
{
    char opponent = (ai_player == 'X') ? 'O' : 'X';

    enum Winner winner = check_winner(board);
    if (winner == X_WIN) {
        return (ai_player == 'X') ? 10 : -10;
    } else if (winner == O_WIN) {
        return (ai_player == 'O') ? 10 : -10;
    } else if (winner == DRAW) {
        return 0;
    }

    int best_value;
    if (ai_turn) {
        best_value = -9999;
        for (int y = 0; y < 3;y++) {
            for (int x = 0; x < 3;x++) {
                if (board[x][y] == ' ') {
                    board[x][y] = ai_player;
                    best_value = max(best_value, minimax(board, ai_player, !ai_turn, alpha, beta));
                    board[x][y] = ' ';

                    alpha = max(alpha, best_value);
                    if (beta <= alpha) {
                        return best_value;
                    }
                }
            }
        }
    } else {
        best_value = 9999;
        for (int y = 0; y < 3;y++) {
            for (int x = 0; x < 3;x++) {
                if (board[x][y] == ' ') {
                    board[x][y] = opponent;
                    best_value = min(best_value, minimax(board, ai_player, !ai_turn, alpha, beta));
                    board[x][y] = ' ';

                    beta = min(beta, best_value);
                    if (beta <= alpha) {
                        return best_value;
                    }
                }
            }
        }
    }
    return best_value;
}
