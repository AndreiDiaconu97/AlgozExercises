#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct Node {
    unsigned int value = 0;
    vector<Node *> vic;
    bool isVisited = false;
    bool deactivated = false;
};

void printGraph(vector<Node> &graph);
void resetVisits(vector<Node> &graph);
void dfs(Node &n, Node &last, set<int> &mySet);

ifstream in("input.txt");
ofstream out("output.txt");

int main() {
    vector<Node> graph;

    unsigned int N, M;
    in >> N >> M;
    graph.resize(N);

    /* push nodes */
    int from, to;
    for (int i = 0; i < M; i++) {
        in >> from >> to;
        graph[from].vic.push_back(&graph[to]);
        graph[to].vic.push_back(&graph[from]);
    }

    /* insert values */
    for (unsigned int k = 0; k < N; k++) {
        graph[k].value = k;
    }

    set<int> mySet;
    for (int k = 0; k < N; k++) {
        if (!graph[k].isVisited) {
            dfs(graph[k], graph[k], mySet);
        }
    }

    out << mySet.size() << " ";
    for (const int &n : mySet) {
        out << n << " ";
    }
    out << "#";

    //printGraph(graph);
    return 0;
}

void dfs(Node &n, Node &last, set<int> &mySet) {
    n.isVisited = true;
    //cout << n.value << endl;

    for (Node *v : n.vic) {
        if (!n.deactivated) {
            if (!v->isVisited) {
                dfs(*v, n, mySet);
            } else {
                if (v != &last) {
                    v->deactivated = true;
                    //cout << v->value << " !" << endl;
                    mySet.insert(v->value);
                }
            }
        }
    }
}

void printGraph(vector<Node> &graph) {
    for (auto &node : graph) {
        out << "N: " << node.value << "\tV: ";
        for (auto &vic : node.vic) {
            out << vic->value << ", ";
        }
        out << endl;
    }
    out << endl;
}

void resetVisits(vector<Node> &graph) {
    for (auto &node : graph) {
        node.isVisited = false;
    }
}