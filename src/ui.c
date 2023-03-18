#include <stdio.h>
#include <stdlib.h>

#include "../include/ui.h"
#include "../include/tictactoe.h"
#include "../include/minimax.h"

static char choose_player();
static void print_board(char board[3][3]);
static struct Move get_user_move();

void play_game()
{
    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    int cur_player = 1;
    int user_player = choose_player();

    while (check_winner(board) == NOT_FINISHED) {
        print_board(board);

        struct Move move;
        if (cur_player == 1) {
            printf("\nPlayer 1 (X) move: ");
            move = (user_player == 'X') ? get_user_move() : find_best_move(board, 'X');
        } else {
            printf("\nPlayer 2 (O) move: ");
            move = (user_player == 'O') ? get_user_move() : find_best_move(board, 'O');
        }
        if (!is_move_valid(board, move)) {
            printf("Invalid move!\n");
            continue;
        }
        board[move.x][move.y] = (cur_player == 1) ? 'X' : 'O';
        cur_player *= -1;
    }
    print_board(board);

    enum Winner winner = check_winner(board);
    if (winner == X_WIN) {
        printf("Player 1 (X) WINS!\n");
    } else if (winner == O_WIN) {
        printf("Player 2 (O) WINS!\n");
    } else {
        printf("It's a DRAW!\n");
    }
}

static char choose_player()
{
    char input[64] = "\0";
    printf("\nDo you want to play as X or O?: ");
    scanf("%s", input);
    if (input[1] != '\0') {
        printf("Invalid input!\n");
        return choose_player();
    }
    if (input[0] == 'X' || input[0] == 'x') {
        return 'X';
    } else if (input[0] == 'O' || input[0] == 'o') {
        return 'O';
    } else {
        printf("Invalid input!\n");
        return choose_player();
    }
}

static struct Move get_user_move()
{
    char input[64] = "\0";
    scanf("%s", input);

    struct Move move = {-1, -1};
    if (input[2] != '\0') {
        return move;
    }
    switch (input[0]) {
    case 'q': case 'Q': exit(0);
    case 'a': case 'A': move.x = 0; break;
    case 'b': case 'B': move.x = 1; break;
    case 'c': case 'C': move.x = 2;
    }
    switch (input[1]) {
    case '1': move.y = 0; break;
    case '2': move.y = 1; break;
    case '3': move.y = 2;
    }
    return move;
}

void print_board(char board[3][3])
{
    printf("\n   A B C\n");
    for (int y = 0; y < 3; y++) {
        printf("%d ", y + 1);
        for (int x = 0; x < 3; x++) {
            printf("|");
            printf("%c", board[x][y]);
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}
