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
    int visit_count;
    double total_reward;
    struct Node *parent;
    struct Move moves[9];
    struct Node *children[9];
};

static double UCT(struct Node* node, double exploration_param);
static bool is_unexplored(struct Node *node);
static struct Node* select(struct Node* root);
static struct Node* expand(struct Node *node, char ai_player);
static int simulate(struct Node *node, char ai_player);
static void backpropagate(struct Node *node, double reward);
static struct Move choose_best_move(struct Node *root);
static void free_tree(struct Node *node);

int mcts_find_best_move(char board[3][3], char ai_player)
{
    // initialize the root node
    struct Node* root = (struct Node*)malloc(sizeof(struct Node));
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            root->board[y][x] = board[y][x];  // copy current board
        }
    }
    get_valid_moves(root->board, root->moves);
    root->visit_count = 0;
    root->total_reward = 0.0;
    root->parent = NULL;
    for (int i = 0; i < 9; i++) {
        root->children[i] = NULL;
    }

    // perform MCTS in limited time
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 2) {
        // select the most promising node using the Upper Confidence Bound for Trees (UCT) formula
        struct Node* current_node = root;
        while (!is_unexplored(current_node)) {
            double best_score = -INFINITY;
            struct Node* next = NULL;

            for (int i = 0; i < 9; i++) {
                struct Node *child = current_node->children[i];
                if (child != NULL) {
                    //printf("child: %d", i);

                    double uct_score = UCT(child, UCT_CONSTANT);
                    //printf("uct_score: %f\n", uct_score);
                    if (uct_score > best_score) {
                        best_score = uct_score;
                        next = child;
                    }
                } else {
                    break;
                }
            }
            current_node = next;
        }

        // expand the selected node if it hasn't been fully expanded
        if (is_unexplored(current_node)) {
            current_node = expand(current_node, ai_player);
        }

        // simulate then backpropagate
        int reward = simulate(current_node, ai_player);
        backpropagate(current_node, reward);
    }

    // choose the best move based on the highest reward
    struct Move best_move = choose_best_move(root);
    printf("visit count: %d\n", root->visit_count);
    printf("reward: %f\n", root->total_reward);
    printf("best move: (x:%d,y:%d)\n", best_move.x, best_move.y);
    struct Move chosen_move;  // copy before freeing
    chosen_move.x = best_move.x;
    chosen_move.y = best_move.y;
    printf("chosen move: (x:%d,y:%d)\n", chosen_move.x, chosen_move.y);
    free_tree(root);  // recursively free the tree
    printf("chosen move: x:%d\n", chosen_move.x);
    int x = chosen_move.x;
    printf("x:%d\n", x);
    return x;
}

// static struct Node* select(struct Node* root)
// {
//     struct Node* node = root;
//     while (!is_unexplored(node->board, node)) {
//         double best_uct = -INFINITY;
//         struct Node* best_child = NULL;
//         for (int i = 0; i < 9; i++) {

//             struct Node* child = node->children[i];
//             if (child != NULL) {
//                 double uct = UCT(child, UCT_CONSTANT);
//                 if (uct > best_uct) {
//                     best_uct = uct;
//                     best_child = child;
//                 }
//             } else {
//                 break;
//             }
//         }
//         node = best_child;
//     }
//     return node;
// }

static struct Node *expand(struct Node *node, char ai_player)
{
    // find an unexplored child node
    int i = 0;
    while (node->children[i] != NULL) {
        i++;
    }

    // create the new child node
    struct Node *child = (struct Node*)malloc(sizeof(struct Node));
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            child->board[y][x] = node->board[y][x];
        }
    }
    for (int y = 0; y < 93; y++) {
        child->moves[y] = node->moves[y];
    }
    child->board[i][i] = ai_player;  // make move on the board
    get_valid_moves(child->board, child->moves);
    child->visit_count = 0;
    child->total_reward = 0.0;
    child->parent = node;
    for (int j = 0; j < 9; j++) {
        child->children[j] = NULL;
    }

    // add the new child node to the parent's list of children
    node->children[i] = child;

    return child;
}

static int simulate(struct Node *node, char ai_player)
{
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
        node->total_reward += reward;
        node = node->parent;
    }
}

static struct Move choose_best_move(struct Node *root)
{
    struct Move best_move = {-1, -1};
    double best_score = -INFINITY;
    for (int i = 0; i < 9; i++) {
        //printf("i: %d\n", i);

        struct Node* child = root->children[i];
        if (child != NULL) {
            //printf("child %d: visit count: %d, reward: %f\n", i, child->visit_count, child->total_reward);
            if (child->total_reward > best_score) {
                best_move = root->moves[i];
                best_score = child->total_reward;
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
    double exploitation = node->total_reward / node->visit_count;
    double exploration = exploration_param * sqrt(log(node->parent->visit_count) / node->visit_count);
    return exploitation + exploration;
}

static bool is_unexplored(struct Node *node)
{
    if (node == NULL) return true;

    int valid_count = 0;  // check valid moves count for current board
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (node->board[y][x] == ' ') {
                valid_count++;
            }
        }
    }
    for (int i = 0; i < valid_count; i++) {
        if (node->children[i] == NULL) {
            return true;
        }
    }
    return false;
}

static void free_tree(struct Node *node)
{
    for (int i = 0; i < 9; i++) {
        if (node->children[i] != NULL) {
            free_tree(node->children[i]);
        }
    }
    free(node);
}