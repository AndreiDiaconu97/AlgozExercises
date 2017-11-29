#include <vector>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

struct Node {
    int value = 0;
    int depth = 0;
    int lowkey = 0;
    int vicNum = 0;
    bool isVisited = false;
    bool onStack = false;
    vector<Node *> vic;
};

void findStrongComps(Node *node, vector<Node> *graph);

int globalDepth = 0;
int tmpMaxConnected = 0;
int maxConnected = 0;
auto globalStack = new stack<int>;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    vector<Node> graph;

    unsigned int M;
    unsigned int N;
    in >> N >> M;
    graph.resize(N);
    unsigned int from, to;

    for (int i = 0; i < M; ++i) {
        in >> from >> to;
        // pushing nodes
        graph[from].vic.push_back(&graph[to]);
        // increasing initialVicNum count
        graph.at(from).vicNum++;
    }
    for (unsigned int k = 0; k < N; ++k) {
        graph.at(k).value = k;

    }

    for (auto &node : graph) {
        // for every non visited node use main algorithm
        if (!node.isVisited) {
            //out << "VISIT" << endl;
            findStrongComps(&node, &graph);
        }
    }
    out << maxConnected;
    return 0;
}

void findStrongComps(Node *node, vector<Node> *graph) {
    node->depth = globalDepth;
    node->lowkey = globalDepth;
    node->isVisited = true;
    globalDepth++;

    globalStack->push(node->value);
    node->onStack = true;

    for (auto &vic : node->vic) {
        if (!vic->isVisited) {
            findStrongComps(vic, graph);
            //node->vicNum++;
            node->lowkey = min(node->lowkey, vic->lowkey);
        } else if (vic->onStack) {
            node->lowkey = min(node->lowkey, vic->depth);
        }
    }
    if (node->lowkey == node->depth) {
        int tmpNodeValue;
        do {
            tmpNodeValue = globalStack->top();
            globalStack->pop();
            graph->at(static_cast<unsigned int>(tmpNodeValue)).onStack = false;
            tmpMaxConnected++;
        } while (node->value != tmpNodeValue);
        maxConnected = max(maxConnected, tmpMaxConnected);
        tmpMaxConnected = 0;
        cout << endl;
    }
}
