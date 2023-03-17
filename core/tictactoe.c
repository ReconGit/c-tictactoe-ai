#include <stdlib.h>

#include "../include/tictactoe.h"

#define MAX_MOVES 10

enum Winner check_winner(char board[3][3])
{
    // check rows
    for (int x = 0; x < 3; x++) {
        if (board[x][0] != ' ' && board[x][0] == board[x][1] && board[x][1] == board[x][2]) {
            return board[x][0] == 'X' ? X_WIN : O_WIN;
        }
    }
    // check columns
    for (int y = 0; y < 3; y++) {
        if (board[0][y] != ' ' && board[0][y] == board[1][y] && board[1][y] == board[2][y]) {
            return board[0][y] == 'X' ? X_WIN : O_WIN;
        }
    }
    // check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0] == 'X' ? X_WIN : O_WIN;
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2] == 'X' ? X_WIN : O_WIN;
    }
    // if no winner and board is full, it's a draw
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (board[x][y] == ' ') {
                return NOT_FINISHED;
            }
        }
    }
    return DRAW;
}

struct Move* get_valid_moves(char board[3][3])
{
    struct Move* moves = malloc(MAX_MOVES * sizeof(struct Move));
    int move_num = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[x][y] == ' ') {
                moves[move_num].x = x;
                moves[move_num].y = y;
                move_num++;
            }
        }
    }
    moves[move_num].x = -1; // mark end
    moves[move_num].y = -1;

    return moves;
}

bool is_move_valid(char board[3][3], struct Move move)
{
    if (move.x < 0 || move.x > 2 || move.y < 0 || move.y > 2) {
        return false;
    }
    return board[move.x][move.y] == ' ';
}
