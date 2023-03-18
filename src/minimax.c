//#include <stdio.h> // for debugging

#include "../include/minimax.h"
#include "../include/tictactoe.h"

static int min(int a, int b) { return (a < b) ? a : b; }
static int max(int a, int b) { return (a > b) ? a : b; }

static int minimax(char board[3][3], char player, bool ai_turn);

struct Move find_best_move(char board[3][3], char player)
{
    int best_value = -9999;
    struct Move best_move;
    for (int y = 0; y < 3;y++) {
        for (int x = 0; x < 3;x++) {
            if (board[x][y] == ' ') {  // calculate score for each valid move
                board[x][y] = player;
                int move_value = minimax(board, player, false);
                board[x][y] = ' ';

                if (move_value > best_value) {
                    best_move.x = x;
                    best_move.y = y;
                    best_value = move_value;
                }
                //printf("%d.Move: %c%d, Value: %d\n", i + 1, moves[i].x + 'A', moves[i].y + 1, moveValue);
            }
        }
    }
    return best_move;
}

static int minimax(char board[3][3], char player, bool ai_turn)
{
    char opponent = (player == 'X') ? 'O' : 'X';

    enum Winner winner = check_winner(board);
    if (winner == X_WIN) {
        return (player == 'X') ? 10 : -10;
    } else if (winner == O_WIN) {
        return (player == 'O') ? 10 : -10;
    } else if (winner == DRAW) {
        return 0;
    }

    int best_value;
    if (ai_turn) {  // branching for all valid moves
        best_value = -9999;
        for (int y = 0; y < 3;y++) {
            for (int x = 0; x < 3;x++) {
                if (board[x][y] == ' ') {
                    board[x][y] = player;
                    best_value = max(best_value, minimax(board, player, !ai_turn));
                    board[x][y] = ' ';
                }
            }
        }
    } else {
        best_value = 9999;
        for (int y = 0; y < 3;y++) {
            for (int x = 0; x < 3;x++) {
                if (board[x][y] == ' ') {
                    board[x][y] = opponent;
                    best_value = min(best_value, minimax(board, player, !ai_turn));
                    board[x][y] = ' ';
                }
            }

        }
    }
    return best_value;
}
