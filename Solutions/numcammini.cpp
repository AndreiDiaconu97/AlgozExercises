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
pair<int, int> dist(Node *nodeFrom, Node *nodeTo);

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<Node> graph;
    int N, M, first, last;
    in >> N >> M >> first >> last;
    graph.resize(N);

    for (int i = 0; i < M; i++) {
        int from, to;
        in >> from >> to;
        graph[from].vic.push_back(&graph[to]);
    }
    for (int j = 0; j < graph.size(); ++j) {
        graph.at(j).value = j;
    }

    //printGraph(&graph);
    pair<int, int> result = dist(&graph.at(first), &graph.at(last));
    out << result.first << " " << result.second;
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

pair<int, int> dist(Node *nodeFrom, Node *nodeTo) {
    int mx = -1;
    int count = 0;
    vector<pair<Node *, int>> distVector; // pair (from, distance)
    nodeFrom->visited = true;
    pair<Node *, int> firstPair = make_pair(nodeFrom, 0);
    distVector.push_back(firstPair);

    for (int i = 0; i < distVector.size(); ++i) {
        // per ogni nodo presente in distVector
        for (Node *nodo : distVector.at(i).first->vic) {
            if ((mx == -1) || (distVector.at(distVector.size() - 1).second <= mx)) {
                nodo->visited = true;
                distVector.emplace_back(nodo, distVector.at(i).second + 1);
                //cout << "node: " << nodo->value << " distance: " << distVector.at(i).second + 1 << endl;
                if (nodo->value == nodeTo->value) {
                    mx = distVector.at(distVector.size() - 1).second;
                    count++;
                }
            }
        }
    }
    pair<int, int> distAndNum = make_pair(mx, count);
    //cout << endl;
    //cout << "max: " << mx << " count: " << count << endl;
    return distAndNum;
}
