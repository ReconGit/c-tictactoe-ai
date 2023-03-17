#include "greatest.h"

#include "../include/tictactoe.h"

TEST test_is_move_valid()
{
    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    struct Move valid_move1 = {0, 0};
    struct Move valid_move2 = {1, 2};
    struct Move invalid_move1 = {-1, -1};
    struct Move invalid_move2 = {-1, -1};
    ASSERT_EQ_FMT(true, is_move_valid(board, valid_move1), "%d");
    ASSERT_EQ_FMT(true, is_move_valid(board, valid_move2), "%d");
    ASSERT_EQ_FMT(false, is_move_valid(board, invalid_move1), "%d");
    ASSERT_EQ_FMT(false, is_move_valid(board, invalid_move2), "%d");
    PASS();
}

TEST test_get_valid_moves()
{
    char board[3][3] = {
        {' ', 'O', 'X'},
        {'X', 'X', 'O'},
        {'O', 'O', 'X'}
    };
    struct Move* moves = get_valid_moves(board);
    struct Move expected_move1 = {0, 0};
    struct Move expected_move2 = {-1, -1};

    ASSERT_EQ_FMT(expected_move1.x, moves[0].x, "%d");
    ASSERT_EQ_FMT(expected_move1.y, moves[0].y, "%d");

    ASSERT_EQ_FMT(expected_move2.x, moves[1].x, "%d");
    ASSERT_EQ_FMT(expected_move2.y, moves[1].y, "%d");
    PASS();
}

GREATEST_SUITE(all_tests)
{
    RUN_TEST(test_is_move_valid);
    RUN_TEST(test_get_valid_moves);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(all_tests);

    GREATEST_MAIN_END();
}
