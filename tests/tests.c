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

GREATEST_SUITE(all_tests)
{
    RUN_TEST(test_is_move_valid);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(all_tests);

    GREATEST_MAIN_END();
}
