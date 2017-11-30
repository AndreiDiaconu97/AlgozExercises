#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Node {
    int depthReversed = 0;
    int value = 0;
    //int vicNum = 0;
    vector<Node *> vic;
    bool isVisited = false;
    bool hasParent = false;
};

void printGraph(vector<Node> *graph);

int maxPath(Node *node);

int maxDepth = 0;

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

    int tmpMax;
    for (auto &node : graph) {
        if (!node.hasParent) {
            tmpMax = maxPath(&node);
            maxDepth = max(maxDepth, tmpMax);
        }
    }
    out << maxDepth - 1;
    //printGraph(&graph);
    return 0;
}

int maxPath(Node *node) {
    if (!node->isVisited) {
        for (auto &vic : node->vic) {
            node->depthReversed = max(node->depthReversed, maxPath(vic));
        }
        node->isVisited = true;
    }
    return node->depthReversed + 1;
}

void printGraph(vector<Node> *graph) {
    for (auto &node : *graph) {
        cout << node.value << ": ";
        for (auto &vic : node.vic) {
            cout << vic->value << " ";
        }
        cout << endl;
    }
    cout << endl;
}
