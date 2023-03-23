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
    struct Node *children[9];
};

double UCT(struct Node* node, double exploration_param);
static bool is_leaf(struct Node *node);
static struct Node *expand(struct Node *node, char ai_player);
static int simulate(struct Node *node, char ai_player);
static void backpropagate(struct Node *node, double reward);
static int choose_best_move(struct Node *root);
static void free_tree(struct Node *node);

struct Move mcts_find_best_move(char board[3][3], char ai_player)
{
    // Initialize the root node
    struct Node* root = (struct Node*)malloc(sizeof(struct Node));
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            root->board[y][x] = board[y][x];  // copy current board
        }
    }
    root->visit_count = 0;
    root->total_reward = 0.0;
    root->parent = NULL;
    for (int i = 0; i < 9; i++) {
        root->children[i] = NULL;
    }

    // limit the search time
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 2) {
        // select the most promising node using the Upper Confidence Bound for Trees (UCT) formula
        struct Node* current_node = root;
        while (!is_leaf(current_node)) {
            double best_score = -INFINITY;
            struct Node* next = NULL;

            for (int i = 0; i < 9; i++) {
                struct Node *child = current_node->children[i];
                if (child != NULL) {

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
        if (is_leaf(current_node)) {
            current_node = expand(current_node, ai_player);
        }

        // simulate then backpropagate
        int reward = simulate(current_node, ai_player);
        backpropagate(current_node, reward);
    }

    // choose the best move based on the number of visits to each child node
    int best_move = choose_best_move(root);
    printf("visit count: %d\n", root->visit_count);
    printf("reward: %f\n", root->total_reward);
    printf("best move: %d\n", best_move);
    struct Move chosen_move;
    chosen_move.y = best_move / 3;
    chosen_move.x = best_move % 3;

    free_tree(root);  // recursively free the tree

    return chosen_move;
}

struct Node* select(struct Node* root)
{
    struct Node* node = root;
    while (!is_leaf(node)) {
        double best_uct = -INFINITY;
        struct Node* best_child = NULL;
        
        for (int i = 0; i < 9; i++) {
            struct Node* child = node->children[i];

            if (child != NULL) {
                double uct = UCT(child, UCT_CONSTANT);
                
                if (uct > best_uct) {
                    best_uct = uct;
                    best_child = child;
                }
            } else {
                break;
            }
        }
        node = best_child;
    }
    return node;
}

double UCT(struct Node* node, double exploration_param)
{
    if (node->visit_count == 0) {
        return INFINITY; // node not yet explored
    }
    double exploitation = node->total_reward / node->visit_count;
    double exploration = exploration_param * sqrt(log(node->parent->visit_count) / node->visit_count);
    return exploitation + exploration;
}

static bool is_leaf(struct Node *node)
{
    for (int i = 0; i < 9; i++) {
        if (node->children[i] != NULL) {
            //printf("not a leaf\n");
            return false;
        }
    }
    //printf("is a leaf\n");
    return true;
}

static struct Node *expand(struct Node *node, char ai_player)
{
    // find an unexplored child node
    int i = 0;
    while (node->children[i] != NULL) {
        i++;
    }
    //if (i==0) printf("nothing to expand\n");

    // create the new child node
    struct Node *child = (struct Node*)malloc(sizeof(struct Node));
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            child->board[y][x] = node->board[y][x];
        }
    }
    child->board[i / 3][i % 3] = ai_player;  // make move on the board
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
    node->total_reward += reward;
    node->visit_count++;
    //printf("%f\n", reward);

    // Recursively update the total reward and visit count for all ancestor nodes
    if (node->parent != NULL) {
        backpropagate(node->parent, reward);
    }
}

static int choose_best_move(struct Node *root)
{
    int best_move = -1;
    int best_visit_count = -1;
    for (int i = 0; i < 9; i++) {
        struct Node* child = root->children[i];

        if (child != NULL) {
            printf("child visit count: %d\n", child->visit_count);
            if (child->visit_count > best_visit_count) {
                best_move = i;
                best_visit_count = child->visit_count;
            }
        } else {
            break;
        }
    }
    return best_move;
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