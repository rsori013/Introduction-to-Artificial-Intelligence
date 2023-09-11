// Created by Abhinav Allam and Rovin Soriano,  NET_ID= aalla009
// https://www.tutorialspoint.com/cplusplus-program-to-implement-a-heuristic-to-find-the-vertex-cover-of-a-graph.
// Utilized ChatGPT for purely debugging purposes.
// https://www.youtube.com/watch?v=dvWk0vgHijs
// https://www.geeksforgeeks.org/vector-in-cpp-stl/

#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include "node.h"

using namespace std;

struct CompareNodes {
    bool operator()(Node* a, Node* b) const {
        if (a->f > b->f) {
            return true;
    }   else {
            return false;
    }
}};

bool ifVisitedChecker(const vector<vector<int>>& state, const vector<vector<vector<int>>>& visitedStates) {
    for (int i = 0; i < visitedStates.size(); ++i) {
        if (visitedStates[i] == state) {
            return true;
        }
    }
    return false;
}


double MisplacedTileCount(const vector<vector<int>>& current, const vector<vector<int>>& goal) {
    int misplacedTiles = 0;
    for (int i = 0; i < current.size(); ++i) {
        for (int j = 0; j < current.size(); ++j) {
            if (current[i][j] != goal[i][j] && current[i][j] != 0) {
                misplacedTiles++;
            }
        }
    }
    return misplacedTiles; // returning how many misplaced tiles there are currently
}

// This function will be looking for the 0 in the puzzle.
pair<int, int> FindEmptyTile(const vector<vector<int>>& state) {
    for (int i = 0; i < state.size(); ++i) {
        for (int j = 0; j < state.size(); ++j) {
            if (state[i][j] == 0) { // meaning we find the empty tile lets gooo and returing its location
                return {i, j};
            }
        }
    }
    cout << "ERROR: Something went wrong" << endl;
    return {-4, -20};
}


// this is the function that will be use to swap
vector<vector<int>> SwappingNum(const vector<vector<int>>& state, int roww, int coll, int row, int col) {
    vector<vector<int>> Puzzle = state;
    swap(Puzzle[roww][coll],Puzzle[row][col]);
    return Puzzle;
}

// To print solution following the structure output given.
void printSolution(Node* goalNode) {
    //it not the root node then there are other nodes to be printed.
    if (goalNode->parent != nullptr) {
        printSolution(goalNode->parent);
    }

    cout << "The best state to expand with g(n) = " << goalNode->g << " and h(n) = " << goalNode->h << " is:\n";
    for (int i = 0; i < goalNode->state.size(); ++i) {
        for (int j = 0; j < goalNode->state[i].size(); ++j) {
            cout << goalNode->state[i][j] << " ";
        }
    cout << endl;
}
cout << endl;
}

vector<Node*> ExpandNode(Node* currNode, const vector<vector<int>>& goal) {
    pair<int, int> emptyTile = FindEmptyTile(currNode->state);
    vector<Node*> children;
    int row = emptyTile.first;
    int col = emptyTile.second;
    //possible operations a puzzle can move to
    vector<pair<int, int>> operations = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < operations.size(); ++i) {
        int newRow = row + operations[i].first;
        int newCol = col + operations[i].second;

        if (newRow >= 0 && newRow < goal.size() && newCol >= 0 && newCol < goal.size()) {
            vector<vector<int>> newPuzzle = SwappingNum(currNode->state, row, col, newRow, newCol);
            Node* child = new Node{newPuzzle, currNode->g + 1, MisplacedTileCount(newPuzzle, goal), 0, currNode};
            child->f = child->g + child->h;
            children.push_back(child);

            // Add the child to the parent's children vector
            currNode->children.push_back(child);
        }
    }

    return children;
}

void AStarMisplaced(vector<vector<int>>& initialState, vector<vector<int>>& goal) {
    priority_queue<Node*, vector<Node*>, CompareNodes> queue;
    Node* root = new Node{initialState, 0, MisplacedTileCount(initialState, goal), 0, nullptr};
    root->f = root->g + root->h;
    queue.push(root);

    int nodes_expanded = 1;
    int max_nodes_in_queue = 1;
    int goal_depth = 0;

    vector<vector<vector<int>>> visitedStates;

    while (!queue.empty()) {
        Node* current = queue.top();
        queue.pop();

        if (current->state == goal) {
            goal_depth = current->g;
            printSolution(current);
            break;
        }

        if (!ifVisitedChecker(current->state, visitedStates)) {
            visitedStates.push_back(current->state);
            vector<Node*> children = ExpandNode(current, goal);
            nodes_expanded++;

            for (Node* child : children) {
                child->f = child->g + child->h;
                queue.push(child);
                current->children.push_back(child); // Add child to the parent's children vector
            }

            max_nodes_in_queue = max(max_nodes_in_queue, static_cast<int>(queue.size()));
        }
    }

    cout << "To solve this problem the search algorithm expanded a total of " << nodes_expanded << " nodes.\n";
    cout << "The maximum number of nodes in the queue at any one time: " << max_nodes_in_queue << ".\n";
    cout << "The depth of the goal node was " << goal_depth << ".\n";
}