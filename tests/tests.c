#include "greatest.h"

#include "../include/tictactoe.h"
#include "../include/minimax.h"

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
    ASSERT_EQ_FMT(true, is_move_valid(board, valid_move1), "%d");
    ASSERT_EQ_FMT(true, is_move_valid(board, valid_move2), "%d");
    ASSERT_EQ_FMT(false, is_move_valid(board, invalid_move1), "%d");
    ASSERT_EQ_FMT(false, is_move_valid(board, invalid_move2), "%d");
    PASS();
}

TEST test_check_winner()
{
    char board[3][3] = {
        {'X', 'X', 'X'},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    ASSERT_EQ_FMTm("X win", X_WIN, check_winner(board), "%d");

    char board2[3][3] = {
        {'X', 'X', 'O'},
        {'O', 'O', 'X'},
        {'X', 'O', 'X'}
    };
    ASSERT_EQ_FMTm("draw", DRAW, check_winner(board2), "%d");
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

SUITE(all_tests)
{
    RUN_TEST(test_is_move_valid);
    RUN_TEST(test_check_winner);
    RUN_TEST(test_find_best_move);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(all_tests);

    GREATEST_MAIN_END();
}
