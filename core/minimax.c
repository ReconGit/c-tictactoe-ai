#include <stdlib.h>
//#include <stdio.h> // for debugging

#include "../include/minimax.h"
#include "../include/tictactoe.h"
#include "../include/ui.h"

int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }

static int minimax(char board[3][3], char player, bool turn);

struct Move find_best_move(char board[3][3], char player)
{
    struct Move* moves = get_valid_moves(board);
    int bestValue = -9999;
    struct Move bestMove = moves[0];

    for (int i = 0; moves[i].x != -1; i++) {
        board[moves[i].x][moves[i].y] = player;
        int moveValue = minimax(board, player, false);
        board[moves[i].x][moves[i].y] = ' ';

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = moves[i];
        }
        //printf("%d.Move: %c%d, Value: %d\n", i + 1, moves[i].x + 'A', moves[i].y + 1, moveValue);
    }
    free(moves);
    return bestMove;
}

static int minimax(char board[3][3], char player, bool turn)
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

    struct Move* moves = get_valid_moves(board);
    int best_value;
    if (turn) {
        best_value = -9999;
        for (int i = 0; moves[i].x != -1; i++) {
            board[moves[i].x][moves[i].y] = player;
            best_value = max(best_value, minimax(board, player, !turn));
            board[moves[i].x][moves[i].y] = ' ';
        }
    } else {
        best_value = 9999;
        for (int i = 0; moves[i].x != -1; i++) {
            board[moves[i].x][moves[i].y] = opponent;
            best_value = min(best_value, minimax(board, player, !turn));
            board[moves[i].x][moves[i].y] = ' ';
        }
    }
    free(moves);
    return best_value;
}
