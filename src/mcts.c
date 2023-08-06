#include "../include/mcts.h"
#include "../include/tictactoe.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//#include <stdio.h> // for debugging

#define ITERATIONS 1000

struct Node {
    char player;
    int wins;
    int visits;
    int children_count;
    struct Move move;
    struct Node* parent;
    struct Node* children[9];
    struct Move unexplored[9];
};

static double UCT(struct Node* node);
static struct Node* select_child(struct Node* root);
static struct Node* expand(struct Node *node, char board[3][3]);
static int simulate(struct Node *node, char board[3][3]);
static void backpropagate(struct Node *node, enum Winner winner);
static struct Move choose_best_move(struct Node *root);
static void free_tree(struct Node *node);

struct Move mcts_move(char board[3][3], char ai_player)
{
    srand((unsigned int)time(NULL));  // set random seed
    // initialize the root node
    struct Node* root = (struct Node*)malloc(sizeof(struct Node));
    root->player = (ai_player == 'X') ? 'O' : 'X';  // root player is the opponent of the AI
    root->children_count = 0;
    get_valid_moves(board, root->unexplored);
    root->visits = 0;
    root->wins = 0.0;
    root->move = (struct Move){-1, -1};
    root->parent = NULL;
    for (int i = 0; i < 9; i++) {
        root->children[i] = NULL;
    }

    for (int i = 0; i < ITERATIONS; i++) {
        struct Node* node = root;
        char simulation[3][3];
        memcpy(simulation, board, sizeof(simulation));

        while (node->unexplored[0].x == -1 && node->children_count > 0) {
            node = select_child(node);
            simulation[node->move.y][node->move.x] = node->player;
        }
        if (check_winner(simulation) == PLAYING) {
            if (node->unexplored[0].x != -1) {
                node = expand(node, simulation);
            }
        }
        enum Winner winner = simulate(node, simulation);
        backpropagate(node, winner);
    }
    // for (int i = 0; i < root->children_count; i++) {
    //     printf("Move: %d %d, wins: %d, visit count: %d\n", root->children[i]->move.x, root->children[i]->move.y, root->children[i]->wins, root->children[i]->visits);
    // }
    // printf("MCTS: %d simulations\n", root->visits);

    // choose the best move based on the highest winner
    struct Move best_move = choose_best_move(root);
    free_tree(root);  // recursively free the tree

    return best_move;
}

static struct Node* select_child(struct Node* node)
{
    double best_uct = -INFINITY;
    struct Node* best_child = node->children[0];

    for (int i = 0; i < node->children_count; i++) {
        struct Node* child = node->children[i];
        double uct = UCT(child);
        if (uct > best_uct) {
            best_uct = uct;
            best_child = child;
        }
    }

    return best_child;
}

static void remove_move(struct Node *node, struct Move move)
{
    for (int i = 0; i < 9; i++) {
        if (node->unexplored[i].x == move.x && node->unexplored[i].y == move.y) {
            // Shift the remaining moves to the left
            int j = i;
            for (; j < 8; j++) {
                node->unexplored[j] = node->unexplored[j + 1];
            }
            node->unexplored[j] = (struct Move){-1, -1};
            break;
        }
    }
}

static struct Node *expand(struct Node *node, char board[3][3])
{
    int i = 0;
    while (node->children[i] != NULL) {
        i++;
    }

    node->children[i] = (struct Node*)malloc(sizeof(struct Node));
    struct Node* child = node->children[i];
    child->player = (node->player == 'X') ? 'O' : 'X';  // set the opposite of previous
    int unexplored = 0;
    for (int i = 0; i < 9; i++) {
        if (node->unexplored[i].x != -1) {
            unexplored++;
        }
    }
    child->move = node->unexplored[rand() % unexplored];
    remove_move(node, child->move);
    board[child->move.y][child->move.x] = child->player;
    get_valid_moves(board, child->unexplored);
    child->children_count = 0;
    child->visits = 0;
    child->wins = 0.0;
    child->parent = node;
    for (int i = 0; i < 9; i++) {
        child->children[i] = NULL;
    }

    node->children_count++;
    node->children[i] = child;

    return child;
}

static int simulate(struct Node *node, char board[3][3])
{
    // simulate a game from the current board state
    char player = (node->player == 'X') ? 'O' : 'X';
    while (check_winner(board) == PLAYING) {
        // choose a random move
        struct Move unexplored[9];
        int move_count = get_valid_moves(board, unexplored);
        struct Move random_move = unexplored[rand() % move_count];

        board[random_move.y][random_move.x] = player;  // make the random move
        player = (player == 'X') ? 'O' : 'X';  // switch to the other player
    }
    return check_winner(board);
}

static void backpropagate(struct Node *node, enum Winner winner)
{
    // update the wins and visit count for the current node
    while (node != NULL) {
        node->visits++;

        if ((winner == X_WIN && node->player == 'X') || (winner == O_WIN && node->player == 'O')) {
            node->wins++;
        } else if ((winner == O_WIN && node->player == 'X') || (winner == X_WIN && node->player == 'O')) {
            node->wins--;
        }
        node = node->parent;
    }
}

static struct Move choose_best_move(struct Node *root)
{
    struct Move best_move = {-1, -1};
    double best_score = -INFINITY;
    for (int i = 0; i < root->children_count; i++) {
        struct Node* child = root->children[i];
        double score = child->visits;
        if (score > best_score) {
            best_move = child->move;
            best_score = score;
        }
    }
    return best_move;
}

static double UCT(struct Node* node)
{
    double exploitation = (double)node->wins / node->visits;
    double exploration = sqrt(2 * log(node->parent->visits) / node->visits);
    return exploitation + exploration;
}

static void free_tree(struct Node *node)
{
    if (node == NULL) return;

    for (int i = 0; i < node->children_count; i++) {
        free_tree(node->children[i]);
    }
    free(node);
}
