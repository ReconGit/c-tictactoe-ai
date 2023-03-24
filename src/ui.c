#include <stdio.h>
#include <stdlib.h>

#include "../include/ui.h"
#include "../include/tictactoe.h"
#include "../include/minimax.h"
#include "../include/mcts.h"

static char choose_player();
static void print_board(char board[3][3]);
static struct Move get_user_move();

void play_game()
{
    char board[3][3] = {
        {' ', ' ', 'X'},
        {' ', 'X', ' '},
        {'O', ' ', 'O'}
    };
    int player_x = 1;
    char user_player = choose_player();

    while (check_winner(board) == PLAYING) {
        print_board(board);

        struct Move move;
        if (player_x == 1) {
            printf((user_player == 'X') ? "Player X turn: " : "Bot X turn: \n");
            move = (user_player == 'X') ? get_user_move() : mcts_find_best_move(board, 'X');
        } else {
            printf((user_player == 'O') ? "Player O turn: " : "Bot O turn: \n");
            move = (user_player == 'O') ? get_user_move() : mcts_find_best_move(board, 'O');
        }
        printf("Move: (x:%c,y:%d)\n", 'A' + move.x, move.y + 1);
        if (!is_move_valid(board, move)) {
            printf("Invalid move!\n");
            continue;
        }
        board[move.y][move.x] = (player_x == 1) ? 'X' : 'O';
        player_x *= -1;
    }
    print_board(board);

    enum Winner winner = check_winner(board);
    if (winner == X_WIN) printf("Bot X WINS!\n\n");
    else if (winner == O_WIN) printf("Bot O WINS!\n\n");
    else printf("It's a DRAW!\n\n");
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
    switch (input[0]) {
    case 'q': case 'Q': exit(0);
    case 'x': case 'X': return 'X'; break;
    case 'o': case 'O': return 'O'; break;
    default:
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

static void print_board(char board[3][3])
{
    printf("\n   A B C\n");
    for (int y = 0; y < 3; y++) {
        printf("%d ", 1 + y);
        for (int x = 0; x < 3; x++) {
            printf("|");
            printf("%c", board[y][x]);
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}
