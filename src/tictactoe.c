#include "../include/tictactoe.h"

#define MAX_MOVES 10

enum Winner check_winner(char board[3][3])
{
    // check rows
    for (int y = 0; y < 3; y++) {
        if (board[y][0] != ' ' && (board[y][0] == board[y][1] && board[y][1] == board[y][2])) {
            return (board[y][0] == 'X') ? X_WIN : O_WIN;
        }
    }
    // check columns
    for (int x = 0; x < 3; x++) {
        if (board[0][x] != ' ' && (board[0][x] == board[1][x] && board[1][x] == board[2][x])) {
            return (board[0][x] == 'X') ? X_WIN : O_WIN;
        }
    }
    // check diagonals
    if (board[0][0] != ' ' && (board[0][0] == board[1][1] && board[1][1] == board[2][2])) {
        return (board[0][0] == 'X') ? X_WIN : O_WIN;
    }
    if (board[0][2] != ' ' && (board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
        return (board[0][2] == 'X') ? X_WIN : O_WIN;
    }
    // if no winner and board is full, it's a draw
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == ' ') {
                return PLAYING;
            }
        }
    }
    return DRAW;
}

int get_valid_moves(char board[3][3], struct Move moves[9])
{
    int move_count = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == ' ') {
                moves[move_count].x = x;
                moves[move_count].y = y;
                move_count++;
            }
        }
    }
    // mark end
    moves[move_count].x = -1;
    moves[move_count].y = -1;

    return move_count;
}

bool is_move_valid(char board[3][3], struct Move move)
{
    if (move.y < 0 || move.y > 2 || move.x < 0 || move.x > 2) {
        return false;
    }
    return board[move.y][move.x] == ' ';
}
