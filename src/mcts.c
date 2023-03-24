#include "../include/mcts.h"
#include "../include/tictactoe.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <stdio.h>

#define SIMULATION_COUNT 1000
#define UCT_CONSTANT 1.41421356237  // sqrt(2)

struct Node {
    char board[3][3];
    struct Move moves[9];
    struct Node* parent;
    struct Node** children;
    int children_count;
    int visit_count;
    double total_score;
};

//static bool is_unexplored(struct Node *node);
static double UCT(struct Node* node, double exploration_param);
static struct Node* select(struct Node* root);
static struct Node* expand(struct Node *node, char ai_player);
static int simulate(struct Node *node, char ai_player);
static void backpropagate(struct Node *node, double reward);
static struct Move choose_best_move(struct Node *root);
static void free_tree(struct Node *node);

struct Move mcts_find_best_move(char board[3][3], char ai_player)
{
    // initialize the root node
    struct Node* root = (struct Node*)malloc(sizeof(struct Node));
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            root->board[y][x] = board[y][x];  // copy current board
        }
    }
    root->children_count = 0;
    root->visit_count = 0;
    root->total_score = 0.0;
    root->parent = NULL;
    root->children = NULL;

    //perform MCTS in limited time
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 2) {
        struct Node* node = select(root);
        struct Node* child = expand(node, ai_player);
        double score = simulate(child, ai_player);
        backpropagate(child, score);
    }

    // choose the best move based on the highest reward
    struct Move best_move = choose_best_move(root);
    printf("visit count: %d\n", root->visit_count);
    printf("reward: %f\n", root->total_score);
    printf("best move: (x:%d,y:%d)\n", best_move.x, best_move.y);
    struct Move chosen_move = {-1, -1};  // copy before freeing
    chosen_move.x = best_move.x;
    chosen_move.y = best_move.y;

    free_tree(root);  // recursively free the tree

    return chosen_move;
}

static struct Node* select(struct Node* root)
{
    struct Node* node = root;
    while (node->children_count > 0) {
        double best_uct = -INFINITY;
        struct Node* best_child = NULL;

        for (int i = 0; i < node->children_count; i++) {
            struct Node* child = node->children[i];
            if (child->visit_count == 0) return child;  // unexplored node
            double uct = UCT(child, UCT_CONSTANT);
            if (uct > best_uct) {
                best_uct = uct;
                best_child = child;
            }
        }
        node = best_child;
    }
    return node;
}

static struct Node *expand(struct Node *node, char ai_player)
{
    int valid_moves_count = get_valid_moves(node->board, node->moves);
    if (valid_moves_count == 0) return NULL;
    node->children_count = valid_moves_count;
    node->children = (struct Node**)malloc(valid_moves_count * sizeof(struct Node*));
    for (int i = 0; i < valid_moves_count; i++) {
        node->children[i] = (struct Node*)malloc(sizeof(struct Node));
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                node->children[i]->board[y][x] = node->board[y][x];
            }
        }
        node->children[i]->board[node->moves[i].y][node->moves[i].x] = ai_player;
        node->children[i]->children_count = 0;
        node->children[i]->visit_count = 0;
        node->children[i]->total_score = 0.0;
        node->children[i]->parent = node;
        node->children[i]->children = NULL;
    }
    int random_index = rand() % valid_moves_count;
    return node->children[random_index];
}

static int simulate(struct Node *node, char ai_player)
{
    if (node == NULL) return 0;
    // copy the board state
    char board[3][3];
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            board[y][x] = node->board[y][x];
        }
    }
    // simulate a game from the current board state
    char current_player = ai_player;
    while (true) {
        enum Winner winner = check_winner(board);
        if (winner == X_WIN) return (ai_player == 'X') ? 1 : -1;
        else if (winner == O_WIN) return (ai_player == 'O') ? 1 : -1;
        else if (winner == DRAW) return 0;

        // choose a random move
        struct Move moves[9];
        int move_count = get_valid_moves(board, moves);
        // printf("move count: %d\n", move_count);
        struct Move random_move = moves[rand() % move_count];

        board[random_move.y][random_move.x] = current_player;  // make random move
        current_player = (current_player == 'X') ? 'O' : 'X';  // switch to the other player
    }
}

static void backpropagate(struct Node *node, double reward)
{
    // Update the total reward and visit count for the current node
    while (node != NULL) {
        node->visit_count++;
        node->total_score += reward;
        node = node->parent;
    }
}

static struct Move choose_best_move(struct Node *root)
{
    struct Move best_move = {-1, -1};
    double best_score = -INFINITY;
    for (int i = 0; i < root->children_count; i++) {
        //printf("i: %d\n", i);

        struct Node* child = root->children[i];
        if (child != NULL) {
            //printf("child %d: visit count: %d, reward: %f\n", i, child->visit_count, child->total_score);
            if (child->total_score > best_score) {
                best_move = root->moves[i];
                best_score = child->total_score;
            }
        } else {
            break;
        }
    }
    return best_move;
}

static double UCT(struct Node* node, double exploration_param)
{
    if (node->visit_count == 0) {
        return INFINITY; // node not yet explored
    }
    double exploitation = node->total_score / node->visit_count;
    double exploration = exploration_param * sqrt(log(node->parent->visit_count) / node->visit_count);
    return exploitation + exploration;
}

static void free_tree(struct Node *node)
{
    if (node == NULL) {
        return;
    }
    if (node->children != NULL) {
        for (int i = 0; i < node->children_count; i++) {
            free_tree(node->children[i]);  // recursively free children
        }
        free(node->children);  // free children array
    }
    free(node);  // free current node
}
