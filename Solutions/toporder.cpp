#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <stack>

using namespace std;

struct Node {
    int value = 0;
    //int vicNum = 0;
    vector<Node *> vic;

    bool isVisited = false;
    bool hasParent = false;

    int lVal = 0;
    //int rVal = 0;
};

void printGraph(vector<Node> *graph);

void topSortSquare(Node *node, int depth);

//set<pair<int, int>> lValNodeValVector+ù

void topSortLinear(Node *node, stack<int> *tmpStack);

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<Node> graph;
    unsigned int N, M;
    in >> N >> M;
    graph.resize(N);
    int from, to;
    for (int i = 0; i < M; ++i) {
        in >> from >> to;
        // pushing nodes
        graph[from].vic.push_back(&graph[to]);
        //graph[from].vicNum++;
        graph[to].hasParent = true;
    }
    for (unsigned int k = 0; k < N; ++k) {
        // inserting values
        graph.at(k).value = k;
    }

    stack<int> tmpStack;
    for (auto &node : graph) {
        if (!node.isVisited) {
            topSortLinear(&node, &tmpStack);
        }
    }

    while (!tmpStack.empty()) {
        out << tmpStack.top() << " ";
        tmpStack.pop();
    }
    /*
    // SCORE: 70
    for (auto &node : graph) {
        pair<int, int> tmpPair = make_pair(node.lVal, node.value);
        lValNodeValVector.insert(tmpPair);
    }
    for (auto &pair : lValNodeValVector) {
        out << pair.second << " ";
    }
    */

    //printGraph(&graph);
    return 0;
}

void topSortLinear(Node *node, stack<int> *tmpStack) {
    if (!node->isVisited) {
        for (auto &vic : node->vic) {
            topSortLinear(vic, tmpStack);
        }
        node->isVisited = true;
        tmpStack->push(node->value);
    }
}

void topSortSquare(Node *node, int depth) {
    node->isVisited = true;
    node->lVal = max(node->lVal, depth);

    depth++;
    for (auto &vic : node->vic) {
        topSortSquare(vic, depth);
    }
}

void printGraph(vector<Node> *graph) {
    for (auto &node : *graph) {
        cout << node.value << ": ";
        for (auto &vic : node.vic) {
            cout << vic->value << " ";
        }
        cout << " lVal:" << node.lVal << endl;
    }
    cout << endl;
}
