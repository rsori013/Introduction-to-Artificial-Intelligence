#ifndef NODE_H
#define NODE_H
#include <vector>
using namespace std;

struct Node {
    vector<vector<int>> state;
    double g;
    double h;
    double f;
    Node* parent;
    vector<Node *> children;
};
#endif // NODE_H