#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>

using namespace std;

struct Node {
    vector<Node *> vic;
    unsigned int value = 0;
    int depth = 0;
    int lowkey = 0;
    int childCount = 0;
    bool isVisited = false;
    bool isArticulation = false;
    bool tmpArticulation = false;

};

void printGraph(vector<Node> *graph);

void setVertexes(Node *node, Node *father, int depth);

void putVicere(vector<Node> *graph);

void resetVisits(vector<Node> *graph);

void findVertexes(Node *node, unsigned int start);

void printVertexes(unordered_set<int> *vertexes);


vector<unsigned int> myVicere;
unordered_set<int> tmpVertexes;

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
        graph[to].vic.push_back(&graph[from]);
        // increasing initialVicNum count

    }
    for (unsigned int k = 0; k < N; ++k) {
        graph.at(k).value = k;

    }

    //printGraph(&graph);
    setVertexes(&graph.at(0), &graph.at(0), 0);
    resetVisits(&graph);
    putVicere(&graph);
    //printGraph(&graph);

    // output vicere
    out << myVicere.size() << endl;
    for (unsigned int k : myVicere) {
        out << k << " ";
    }
    return 0;
}

void putVicere(vector<Node> *graph) {
    for (auto &node: *graph) {
        findVertexes(&node, node.value);
        if (tmpVertexes.size() == 1) {
            myVicere.push_back(node.value);
        }
        tmpVertexes.clear();
    }
}

void findVertexes(Node *node, unsigned int start) {
    if (node->isArticulation) {
        if (node->value != start) {
            int value = node->value;
            tmpVertexes.insert(value);
            //printVertexes(&tmpVertexes);
        }
    } else if (!node->isVisited) {
        node->isVisited = true;
        //cout << "N:" << node->value << endl;
        for (auto &vic : node->vic) {
            findVertexes(vic, start);
        }

    }
}

void printVertexes(set<int> *vertexes) {
    for (auto &vertex : *vertexes) {
        cout << vertex << ",";
    }
    cout << endl;
}

void resetVisits(vector<Node> *graph) {
    for (auto &node : *graph) {
        node.isVisited = false;
    }
}

void setVertexes(Node *node, Node *father, int depth) {
    node->depth = depth;
    node->lowkey = depth;
    node->isVisited = true;

    for (auto &vicino : node->vic) {
        if (!vicino->isVisited) {
            //cout << "visit " << vicino->value << endl;
            setVertexes(vicino, node, depth + 1);
            node->childCount++;
            if (vicino->lowkey >= node->depth) {
                node->tmpArticulation = true;
            }
            node->lowkey = min(node->lowkey, vicino->lowkey);
        } else if (vicino != father) {
            //out << "N:" << node->value << " L:" << node->lowkey;
            node->lowkey = min(node->lowkey, vicino->depth);
            //cout << "->" << node->lowkey << endl;
        }

    }
    if (((father != node) && (node->tmpArticulation)) || ((father == node) && (node->childCount > 1))) {
        node->isArticulation = true;
    }
}



