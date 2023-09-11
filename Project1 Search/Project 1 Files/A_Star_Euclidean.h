// Created by Majd Kawak 862273310
// Utilized Lectures slides, ChatGPT and youtube.

#ifndef A_STAR_EUCLIDEAN_H
#define A_STAR_EUCLIDEAN_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <queue>
#include <numeric>
#include "node.h"

using namespace std;

// Goal state needs to be modified based on input
vector<vector<int>> goal_state;

// Save all states
unordered_map<string, Node *> node_map;

// Nodes explored
int nodes_explored = 0;

// Converts state to string
string state_to_string(Node *node) {
    vector<vector<int>> state = node->state;
    int counter = 0;
    int last_element = pow(state.size(), 2) - 1;
    string str = "";
    for (int i = 0; i < state.size(); i++) {
        for (int j = 0; j < state[i].size(); j++) {
            if (counter == last_element) {
                str += to_string(state[i][j]);
            } else {
                str += (to_string(state[i][j]) + ",");
            }
            counter++;
        }
    }
    return str;
}

// Return is a state exists or not
bool state_exists(Node *node) {
    string state_str = state_to_string(node);
    return node_map.find(state_str) != node_map.end();
}

// Print 2D Matrix
void print_state_vec(vector<vector<int>> &vec) {
    for (const auto &row: vec) {
        for (const auto &val: row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Initialize goal state based on input
void initialize_goal_state(int size) {
    goal_state.resize(size);
    int lastVal = size * size - 1;
    for (int i = 0; i < size; i++) {
        goal_state[i].resize(size);
        iota(goal_state[i].begin(), goal_state[i].end(), i * size + 1);
    }
    goal_state[size - 1][size - 1] = 0;
}

// Print state and its f, g and h
void print_node(Node *node) {
    // Print the state of the node
    cout << "State:" << endl;
    for (auto row: node->state) {
        for (auto tile: row) {
            cout << tile << " ";
        }
        cout << endl;
    }
    // Print the heuristic values of the node
    cout << "Heuristic values:" << endl;
    cout << "f = " << node->f << ", g = " << node->g << ", h = " << node->h << ", children count = "
         << node->children.size() << endl;
}

// Print tree
void print_full_tree(Node *node, int depth, char name) {
    // print out the current node's state
    cout << "Level " << depth << " Node " << name << " State:" << endl;
    print_node(node);
    // print out the node's children (if any)
    if (node->children.empty()) {
        return;
    }
    cout << "Children: ";
    for (int i = 0; i < node->children.size(); i++) {
        cout << "Node " << char(name + i + 1) << ", ";
    }
    cout << endl;
    // recursively print out each child node
    for (int i = 0; i < node->children.size(); i++) {
        print_full_tree(node->children[i], depth + 1, char(name + i + 1));
    }
}

// Prints child nodes
void print_child_nodes(Node *node) {
    vector<Node *> child_nodes = node->children;
    for (int i = 0; i < child_nodes.size(); i++) {
        cout << "Child " << i + 1 << ":" << endl;
        cout << "State: " << endl;
        for (int j = 0; j < child_nodes[i]->state.size(); j++) {
            for (int k = 0; k < child_nodes[i]->state[j].size(); k++) {
                cout << child_nodes[i]->state[j][k] << " ";
            }
            cout << endl;
        }
        cout << "f: " << child_nodes[i]->f << endl;
        cout << "g: " << child_nodes[i]->g << endl;
        cout << "h: " << child_nodes[i]->h << endl;
        cout << endl;
    }
}

// Helper comparison function to order the nodes by f value in p_q
struct NodeCompare {
    bool operator()(Node *a, Node *b) {
        return a->f > b->f;
    }
};

// Gets tile position in a state
pair<int, int> get_tile_position(vector<vector<int>> &state, int tile) {
    for (int i = 0; i < state.size(); i++) {
        for (int j = 0; j < state[i].size(); j++) {
            if (state[i][j] == tile) {
                return make_pair(i, j);
            }
        }
    }
};

// Compute Euclidean Heuristic value for a state
double euclidean_distance_heuristic(vector<vector<int>> &state) {
    double distance = 0;
    for (int i = 0; i < state.size(); i++) {
        for (int j = 0; j < state[i].size(); j++) {
            int tile = state[i][j];
            if (tile != 0) {
                pair<int, int> goal_pos = get_tile_position(goal_state, tile);
                double tile_dis = sqrt(pow(i - goal_pos.first, 2) + pow(j - goal_pos.second, 2));
                distance += tile_dis;
            }
        }
    }
    return distance;
}

// Compute Misplaced Tiles Heuristic value for a state
double misplaced_tiles_heuristic(vector<vector<int>> &state) {
    double distance = 0;
    for (int i = 0; i < state.size(); i++) {
        for (int j = 0; j < state[i].size(); j++) {
            if (state[i][j] != 0) {
                if (state[i][j] != goal_state[i][j]) {
                    distance += 1;  // Add 1 for each misplaced tile
                }
            }
        }
    }
    return distance;
}

// Compute child nodes based on Euclidean Heuristic
void compute_childs_euclidean(Node *node) {

    // Get blank "zero" tile position
    pair<int, int> zero_pos = get_tile_position(node->state, 0);

    // Generate child nodes by applying each valid move
    if (zero_pos.first > 0) {  // Move blank tile up
        Node *up_state = new Node();
        up_state->state = node->state;
        up_state->parent = node;
        swap(up_state->state[zero_pos.first][zero_pos.second],
             up_state->state[zero_pos.first - 1][zero_pos.second]);
        if (!state_exists(up_state)) {
            node_map[state_to_string(up_state)] = up_state;
            up_state->h = euclidean_distance_heuristic(up_state->state);
            up_state->g = node->g + 1;
            up_state->f = up_state->g + up_state->h;
            node->children.push_back(up_state);
        } else {
            delete up_state;
        }
    }
    if (zero_pos.first < node->state.size() - 1) {  // Move blank tile down
        Node *down_state = new Node();
        down_state->state = node->state;
        down_state->parent = node;
        swap(down_state->state[zero_pos.first][zero_pos.second],
             down_state->state[zero_pos.first + 1][zero_pos.second]);
        if (!state_exists(down_state)) {
            node_map[state_to_string(down_state)] = down_state;
            down_state->h = euclidean_distance_heuristic(down_state->state);
            down_state->g = node->g + 1;
            down_state->f = down_state->g + down_state->h;
            node->children.push_back(down_state);
        } else {
            delete down_state;
        }
    }
    if (zero_pos.second > 0) {  // Move blank tile left
        Node *left_state = new Node();
        left_state->state = node->state;
        left_state->parent = node;
        swap(left_state->state[zero_pos.first][zero_pos.second],
             left_state->state[zero_pos.first][zero_pos.second - 1]);
        if (!state_exists(left_state)) {
            node_map[state_to_string(left_state)] = left_state;
            left_state->h = euclidean_distance_heuristic(left_state->state);
            left_state->g = node->g + 1;
            left_state->f = left_state->g + left_state->h;
            node->children.push_back(left_state);
        } else {
            delete left_state;
        }
    }
    if (zero_pos.second < node->state.size() - 1) {  // Move blank tile right
        Node *right_state = new Node();
        right_state->state = node->state;
        right_state->parent = node;
        swap(right_state->state[zero_pos.first][zero_pos.second],
             right_state->state[zero_pos.first][zero_pos.second + 1]);
        if (!state_exists(right_state)) {
            node_map[state_to_string(right_state)] = right_state;
            right_state->h = euclidean_distance_heuristic(right_state->state);
            right_state->g = node->g + 1;
            right_state->f = right_state->g + right_state->h;
            node->children.push_back(right_state);
        } else {
            delete right_state;
        }
    }
}

// Compute child nodes based on Misplaced heuristic
void compute_childs_misplaced(Node *node) {

    // Get blank "zero" tile position
    pair<int, int> zero_pos = get_tile_position(node->state, 0);

    // Generate child nodes by applying each valid move
    if (zero_pos.first > 0) {  // Move blank tile up
        Node *up_state = new Node();
        up_state->state = node->state;
        up_state->parent = node;
        swap(up_state->state[zero_pos.first][zero_pos.second],
             up_state->state[zero_pos.first - 1][zero_pos.second]);
        if (!state_exists(up_state)) {
            node_map[state_to_string(up_state)] = up_state;
            up_state->h = misplaced_tiles_heuristic(up_state->state);
            up_state->g = node->g + 1;
            up_state->f = up_state->g + up_state->h;
            node->children.push_back(up_state);
        } else {
            delete up_state;
        }
    }
    if (zero_pos.first < node->state.size() - 1) {  // Move blank tile down
        Node *down_state = new Node();
        down_state->state = node->state;
        down_state->parent = node;
        swap(down_state->state[zero_pos.first][zero_pos.second],
             down_state->state[zero_pos.first + 1][zero_pos.second]);
        if (!state_exists(down_state)) {
            node_map[state_to_string(down_state)] = down_state;
            down_state->h = misplaced_tiles_heuristic(down_state->state);
            down_state->g = node->g + 1;
            down_state->f = down_state->g + down_state->h;
            node->children.push_back(down_state);
        } else {
            delete down_state;
        }
    }
    if (zero_pos.second > 0) {  // Move blank tile left
        Node *left_state = new Node();
        left_state->state = node->state;
        left_state->parent = node;
        swap(left_state->state[zero_pos.first][zero_pos.second],
             left_state->state[zero_pos.first][zero_pos.second - 1]);
        if (!state_exists(left_state)) {
            node_map[state_to_string(left_state)] = left_state;
            left_state->h = misplaced_tiles_heuristic(left_state->state);
            left_state->g = node->g + 1;
            left_state->f = left_state->g + left_state->h;
            node->children.push_back(left_state);
        } else {
            delete left_state;
        }
    }
    if (zero_pos.second < node->state.size() - 1) {  // Move blank tile right
        Node *right_state = new Node();
        right_state->state = node->state;
        right_state->parent = node;
        swap(right_state->state[zero_pos.first][zero_pos.second],
             right_state->state[zero_pos.first][zero_pos.second + 1]);
        if (!state_exists(right_state)) {
            node_map[state_to_string(right_state)] = right_state;
            right_state->h = misplaced_tiles_heuristic(right_state->state);
            right_state->g = node->g + 1;
            right_state->f = right_state->g + right_state->h;
            node->children.push_back(right_state);
        } else {
            delete right_state;
        }
    }
}

// Checks is it's a goal state
bool is_goal_state(vector<vector<int>> state) {
    // calculate the expected last value
    int last = state.size() * state[0].size() - 1;
    // start counting from 1
    int count = 1;
    for (auto row: state) {
        for (auto val: row) {
            if (val != count && count <= last) {  // check if the current value is correct
                return false;
            }
            count++;
        }
    }
    return true;
}

// Prints frontier
void print_priority_queue(priority_queue<Node *, vector<Node *>, NodeCompare> &frontier) {
    priority_queue<Node *, vector<Node *>, NodeCompare> temp = frontier;
    while (!temp.empty()) {
        Node *node = temp.top();
        print_node(node);
        temp.pop();
    }
}

// Push child nodes onto p_q
void push_child_nodes_into_priority_queue(Node *node, priority_queue<Node *, vector<Node *>, NodeCompare> &frontier) {
    vector<Node *> child_nodes = node->children;
    for (int i = 0; i < child_nodes.size(); i++) {
        frontier.push(child_nodes[i]);
    }
}

// Print path from root to solution
void print_solution_path(Node *node) {
    vector<Node *> path;
    while (node != nullptr) {
        path.push_back(node);
        node = node->parent;
    }
    int level = 0;
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "Depth " << level << endl;
        print_node(path[i]);
        level++;
        cout << endl;
    }
}

// Function to count expanded states
int countNodes(Node *node) {
    int count = 1;  // count the current node
    for (auto child: node->children) {
        count += countNodes(child);  // recursively count the child nodes
    }
    return count;
}

// Solve puzzle using Euclidean heuristic
bool solve_puzzle_euclidean(Node *node) {

    // Initialize goal state
    initialize_goal_state(node->state.size());
    // Insert state in node_map
    node_map[state_to_string(node)] = node;
    // Priority Queue "Frontier"
    priority_queue<Node *, vector<Node *>, NodeCompare> frontier;
    frontier.push(node);
    while (!frontier.empty()) {
        // Explore Min state
        Node *current_node = frontier.top();
        frontier.pop();
        if (is_goal_state(current_node->state)) {
            // Solution found
            cout << "Goal Found" << endl;
            cout << "-------------------------------" << endl;
            print_node(current_node);
            cout << "-------------------------------" << endl;
            print_solution_path(current_node);
            cout << "-------------------------------" << endl;
            print_full_tree(node, 0, 'A');
            cout << "-------------------------------" << endl;
            return true;
        }
        // Compute Child Nodes
        compute_childs_euclidean(current_node);
        // Push onto the queue
        push_child_nodes_into_priority_queue(current_node, frontier);
        nodes_explored++;
    }
    // No solution found
    return false;
}

// Solve puzzle using Misplaced heuristic
bool solve_puzzle_misplaced(Node *node) {

    // Initialize goal state
    initialize_goal_state(node->state.size());
    // Insert state in node_map
    node_map[state_to_string(node)] = node;
    // Priority Queue "Frontier"
    priority_queue<Node *, vector<Node *>, NodeCompare> frontier;
    frontier.push(node);

    while (!frontier.empty()) {
        // Explore Min state
        Node *current_node = frontier.top();
        frontier.pop();
        if (is_goal_state(current_node->state)) {
            // Solution found
            cout << "Goal Found" << endl;
            cout << "-------------------------------" << endl;
            print_node(current_node);
            cout << "-------------------------------" << endl;
            print_solution_path(current_node);
            cout << "-------------------------------" << endl;
            print_full_tree(node, 0, 'A');
            cout << "-------------------------------" << endl;
            return true;
        }
        // Compute Child Nodes
        compute_childs_misplaced(current_node);
        // Push onto the queue
        push_child_nodes_into_priority_queue(current_node, frontier);
        nodes_explored++;
    }
    // No solution found
    return false;
}

void A_Star_Euclidean(vector<vector<int>> &init_puzzle, vector<vector<int>> &goal) {

    Node *puzzle = new Node();
    puzzle->state = init_puzzle;

    if (!solve_puzzle_euclidean(puzzle)) {
        std::cout << "No solution found." << std::endl;
    }
    cout << "Number of node = " << countNodes(puzzle) << endl;
    cout << "Nodes explored = " << nodes_explored << endl;

//    if (!solve_puzzle_misplaced(puzzle)) {
//        std::cout << "No solution found." << std::endl;
//    }
//    cout << "Number of node = " << countNodes(puzzle) << endl;
//    cout << "Nodes explored = " << nodes_explored << endl;
}

#endif //A_STAR_EUCLIDEAN_H
