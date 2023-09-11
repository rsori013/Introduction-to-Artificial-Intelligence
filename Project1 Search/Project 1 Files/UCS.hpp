// Created by Jacqueline Gardea 862305608

#include <queue>
#include <vector>
#include <algorithm>
#include <utility>
#include "ucNode.h"
#include "Problem.h"
#include <iostream>

using namespace std;

void print(const vector<vector<int>> &puzzle) {
    for (const auto &row : puzzle) {
        for (const auto &val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

ucNode* ucs(Problem* p){
    ucNode* start = p->initialState;
    int row = start->yB;
    int col = start->xB;
    int width = p->goalState.size();
    int height = p->goalState[0].size();

    //queue to iterate through
    priority_queue <ucNode*, vector<ucNode*>, compare> queue;
    //a vector (for hash) of vectors(for multiple values) or vector<vector<int>> for puzzles
    vector<vector<vector<vector<int>>>> previousStates(width*height);

    //initialize queue with the start state and add to previous states
    queue.push(start);
    previousStates[row*width + col].push_back(start->state);
    ucNode* current;

    //while the queue of untested states is not empty keep going
    while(!queue.empty()){
        //find the size of queue to get Max Queue Size
        if(queue.size() > p->maxQueueSize){p->maxQueueSize = queue.size();} 

        //pop the top state to check node
        current = queue.top();
        queue.pop();

        //if the state is the goal state then return the state
        if(current->state == p->goalState){
            return current;
        }
        //if the state is not the goal state
        else{
            cout << "The best state to expand with g(n) = " << current->g_n << " and h(n) = 0 is:\n";
            print(current->state);

            //expand the node using the operators
            int x = current->xB;
            int y = current->yB;
            vector<vector<int>> operators = {{0,1},{0,-1},{1,0},{-1,0}};
            for(int i = 0; i < operators.size(); i++){
                int newY = y + operators[i][0];
                int newX = x + operators[i][1];

                //only use the new operator if it is valid
                if((newY < height) && (newX < width) && (newX >= 0) && (newY >= 0)){
                    //get new state of vector with swapped values
                    vector<vector<int>> newState = current->state;
                    swap(newState[y][x], newState[newY][newX]);   

                    //check if this new state has already been pushed into the queue before
                    if(!(find(previousStates[newY*width + newX].begin(), 
                               previousStates[newY*width + newX].end(), newState) != previousStates[newY*width+ newX].end())){

                        //if the new state has not been pushed in create a new node state for it 
                        ucNode* node = new ucNode(current, 
                                            newState,
                                            newY,
                                            newX,
                                            current->g_n + 1,
                                            current->depth + 1);

                        //add node to current' children.
                        current->children.push_back(node);

                        //add node to visited state and queue
                        previousStates[newY*width + newX].push_back(node->state);
                        queue.push(node);
                    }
                }   
            }
        }
        p->expandedNodes++;
    }
    return nullptr;
}