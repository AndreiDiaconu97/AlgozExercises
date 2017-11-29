#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Node {
    bool visited = false;
    vector<Node *> vic;
    int value;
};

int maxMinDistance(vector<Node> *_graph);
int dist(Node *nodo);
void printGraph(vector<Node> *_graph);

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<Node> graph;
    int N, M;
    in >> N >> M;
    graph.resize(N);

    for (int i = 0; i < M; i++) {
        int from, to;
        in >> from >> to;
        graph[from].vic.push_back(&graph[to]);
        graph[to].vic.push_back(&graph[from]); // because our graph is bidirectional
    }
    for (int j = 0; j < graph.size(); ++j) {
        graph.at(j).value = j;
    }

    //printGraph(&graph);
    out << maxMinDistance(&graph);
    return 0;
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
int maxMinDistance(vector<Node> *_graph) {
    int mx = 0;
    for (Node nodo : *_graph) {
        mx = max(dist(&nodo), mx);
        for (int i = 0; i < _graph->size(); ++i) {
            _graph->at(i).visited = false;
        }
    }
    return mx;
}
int dist(Node *from) {
    vector<pair<Node *, int>> distVector; // pair (from, distance)
    from->visited = true;
    pair<Node *, int> firstPair = make_pair(from, 0);
    distVector.push_back(firstPair);

    for (int i = 0; i < distVector.size(); ++i) {
        // per ogni nodo presente in distVector
        for (Node *nodo : distVector.at(i).first->vic) {
            if (!nodo->visited) {
                nodo->visited = true;

                distVector.push_back(pair<Node *, int>(nodo, distVector.at(i).second + 1));
                //cout << "node: " << nodo->value << " distance: " << distVector.at(i).second + 1 << endl;
            }
        }
    }
    //cout << endl;
    return distVector.at(distVector.size() - 1).second;
}
