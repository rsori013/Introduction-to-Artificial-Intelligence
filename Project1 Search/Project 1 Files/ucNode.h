// Created by Jacqueline Gardea 862305608

#ifndef UCNODE_H
#define UCNODE_H
#include <vector>

using namespace std;

struct ucNode{
    ucNode* parent = nullptr;
    vector<vector<int>> state;
    int xB = 0;
    int yB = 0;
    int g_n = 0;
    int depth = 0;
    vector<ucNode*> children;
    ucNode(ucNode* p, vector<vector<int>> s, int y, int x, int g, int d){
        this->parent = p;
        this->state = s;
        this->yB = y;
        this->xB = x;
        this->g_n = g;
        this->depth = d;
    }
};

struct compare{bool operator()(const ucNode* l, const ucNode* r)const { return l->g_n > r->g_n;}};


#endif