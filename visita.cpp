#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Node {
    bool visited = false;
    vector<Node *> vic;
    int value;
};

void printGraph(vector<Node> *_graph);

void visit(Node *node);

int reachable = 0;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<Node> graph;
    int N, M, target;
    in >> N >> M >> target;
    graph.resize(N);

    for (int i = 0; i < M; i++) {
        int from, to;
        in >> from >> to;
        graph[from].vic.push_back(&graph[to]);
    }
    for (int j = 0; j < graph.size(); ++j) {
        graph.at(j).value = j;
    }
    
    visit(&graph.at(target));
    //printGraph(&graph);
    out << reachable;
    return 0;
}

void visit(Node *node) {
    node->visited = true;
    reachable++;

    for (auto &vic : node->vic) {
        if (!vic->visited) {
            visit(vic);
        }
    }
}

void printGraph(vector<Node> *_graph) {
    for (Node n1 : *_graph) {
        cout << n1.value << ": ";
        for (Node *n2 : n1.vic) {
            cout << n2->value << ",";
        }
        cout << endl;
    }
}
