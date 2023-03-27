#include "greatest.h"

#include "../include/tictactoe.h"
#include "../include/minimax.h"
#include "../include/mcts.h"

TEST test_is_move_valid()
{
    char board[3][3] = {
        {'X', 'O', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    struct Move valid_move1 = {2, 0};
    struct Move valid_move2 = {1, 2};
    struct Move invalid_move1 = {-1, -1};
    struct Move invalid_move2 = {0, 0};
    ASSERTm("valid move1", is_move_valid(board, valid_move1));
    ASSERTm("valid move2", is_move_valid(board, valid_move2));
    ASSERT_FALSEm("invalid move1", is_move_valid(board, invalid_move1));
    ASSERT_FALSEm("invalid move2", is_move_valid(board, invalid_move2));
    PASS();
}

TEST test_check_winner()
{
    char board1[3][3] = {
        {'X', 'X', 'O'},
        {'O', 'O', 'X'},
        {' ', 'O', 'X'}
    };
    ASSERT_EQ_FMTm("not finished", PLAYING, check_winner(board1), "%d");

    char board2[3][3] = {
        {'X', 'X', 'X'},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    ASSERT_EQ_FMTm("X win", X_WIN, check_winner(board2), "%d");

    char board3[3][3] = {
        {'O', 'X', 'X'},
        {' ', 'O', 'X'},
        {' ', ' ', 'O'}
    };
    ASSERT_EQ_FMTm("O win", O_WIN, check_winner(board3), "%d");

    char board4[3][3] = {
        {'X', 'X', 'O'},
        {'O', 'O', 'X'},
        {'X', 'O', 'X'}
    };
    ASSERT_EQ_FMTm("draw", DRAW, check_winner(board4), "%d");
    PASS();
}

TEST test_find_best_move()
{
    char board1[3][3] = {
        {'X', 'X', ' '},
        {' ', 'O', 'O'},
        {' ', ' ', ' '}
    };
    struct Move best_move = find_best_move(board1, 'X');
    struct Move best_move2 = mcts_find_best_move(board1, 'X');

    ASSERT_EQ_FMTm("MCTS 1.x coordinate", 2, best_move2.x, "%d");
    ASSERT_EQ_FMTm("MCTS 1.y coordinate", 0, best_move2.y, "%d");

    ASSERT_EQ_FMTm("1.x coordinate", 2, best_move.x, "%d");
    ASSERT_EQ_FMTm("1.y coordinate", 0, best_move.y, "%d");
    best_move = find_best_move(board1, 'O');
    ASSERT_EQ_FMTm("2.x coordinate", 0, best_move.x, "%d");
    ASSERT_EQ_FMTm("2.y coordinate", 1, best_move.y, "%d");

    char board2[3][3] = {
        {'O', 'O', ' '},
        {' ', 'X', 'X'},
        {' ', ' ', ' '}
    };
    best_move = find_best_move(board2, 'X');
    ASSERT_EQ_FMTm("3.x coordinate", 0, best_move.x, "%d");
    ASSERT_EQ_FMTm("3.y coordinate", 1, best_move.y, "%d");
    best_move = find_best_move(board2, 'O');
    ASSERT_EQ_FMTm("4.x coordinate", 2, best_move.x, "%d");
    ASSERT_EQ_FMTm("4.y coordinate", 0, best_move.y, "%d");

    char board3[3][3] = {
        {'O', ' ', 'X'},
        {' ', 'X', 'O'},
        {' ', ' ', 'O'}
    };
    best_move = find_best_move(board3, 'X');
    ASSERT_EQ_FMTm("5.x coordinate", 0, best_move.x, "%d");
    ASSERT_EQ_FMTm("5.y coordinate", 2, best_move.y, "%d");
    best_move = find_best_move(board3, 'O');
    ASSERT_EQ_FMTm("6.x coordinate", 0, best_move.x, "%d");
    ASSERT_EQ_FMTm("6.y coordinate", 2, best_move.y, "%d");
    PASS();
}

TEST test_valid_moves()
{
    char board[3][3] = {
        {'X', 'X', ' '},
        {' ', 'O', 'O'},
        {' ', ' ', ' '}
    };

    struct Move moves[9];
    int move_count = get_valid_moves(board, moves);
    ASSERT_EQ_FMTm("Valid move count should equal 5", 5, move_count, "%d");
    PASS();
}

TEST self_play()
{
    int games_limit = 20;
    int games_played = 0;
    int minimax_wins = 0;
    int mcts_wins = 0;
    int draws = 0;
    do {
        char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
        };
        int player_x = 1;

        while (check_winner(board) == PLAYING) {
            struct Move move;
            if (player_x == 1) {
                move = find_best_move(board, 'X');
            } else {
                move = mcts_find_best_move(board, 'O');
            }
            if (!is_move_valid(board, move)) {
                printf("Invalid move!\n");
                continue;
            }
            board[move.y][move.x] = (player_x == 1) ? 'X' : 'O';
            player_x *= -1;
        }
        enum Winner winner = check_winner(board);
        if (winner == X_WIN) minimax_wins++;
        else if (winner == O_WIN) mcts_wins++;
        else draws++;
        games_played++;

    } while (games_played < games_limit);
    printf("X minimax wins: %d\n", minimax_wins);
    printf("O mcts wins: %d\n", mcts_wins);
    printf("draws: %d\n\n", draws);

    games_played = 0;
    minimax_wins = 0;
    mcts_wins = 0;
    draws = 0;
    do {
        char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
        };
        int player_x = 1;

        while (check_winner(board) == PLAYING) {
            struct Move move;
            if (player_x == 1) {
                move = mcts_find_best_move(board, 'X');
            } else {
                move = find_best_move(board, 'O');
            }
            if (!is_move_valid(board, move)) {
                printf("Invalid move!\n");
                continue;
            }
            board[move.y][move.x] = (player_x == 1) ? 'X' : 'O';
            player_x *= -1;
        }
        enum Winner winner = check_winner(board);
        if (winner == X_WIN) mcts_wins++;
        else if (winner == O_WIN) minimax_wins++;
        else draws++;
        games_played++;

    } while (games_played < games_limit);
    printf("O minimax wins: %d\n", minimax_wins);
    printf("X mcts wins: %d\n", mcts_wins);
    printf("draws: %d\n", draws);
    PASS();
}

SUITE(all_tests)
{
    RUN_TEST(test_is_move_valid);
    RUN_TEST(test_check_winner);
    RUN_TEST(test_find_best_move);
    RUN_TEST(test_valid_moves);
    RUN_TEST(self_play);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(all_tests);

    GREATEST_MAIN_END();
}
