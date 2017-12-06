#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
#include <set>

using namespace std;

struct Edge;

struct Node {
    unsigned int value = 0;
    int dist = INT32_MAX;
    //unsigned int minTime = INT32_MAX;
    vector<Edge> vic;
    Node *prev = nullptr;
    bool isVisited = false;
};

struct Edge {
    unsigned int w, f, y, n = 0;
    Node *node;
};

void printGraph(vector<Node> *graph);

void findShortestPath(Node *starting);

void printDistVector(vector<pair<Node *, int>> *distVector);

void getShortestPath(Node *node, stack<int> *path);

void printPath(stack<int> *path);

void printSet(set<pair<int, Node *>> *mySet);

ifstream in("input.txt");
ofstream out("output.txt");

int main() {
    unsigned int N, M;
    in >> N >> M;
    vector<Node> graph(N);
    unsigned int s, t, w, f, y, n;

    Edge tmpEdge;
    for (int i = 0; i < M; ++i) {
        in >> s >> t >> w >> f >> y >> n;
        // pushing nodes
        tmpEdge.w = w;
        tmpEdge.f = f;
        tmpEdge.y = y;
        tmpEdge.n = n;

        if (w <= y) {
            tmpEdge.node = &graph[t];
            graph[s].vic.push_back(tmpEdge);

            tmpEdge.node = &graph[s];
            graph[t].vic.push_back(tmpEdge);
        }
    }
    // inserting values
    for (unsigned int k = 0; k < N; ++k) {
        graph.at(k).value = k;
    }

    findShortestPath(&graph[0]);

    stack<int> path;
    getShortestPath(&graph[N - 1], &path);
    if (path.size() > 1) {
        out << graph[N - 1].dist << endl;
        printPath(&path);
    } else {
        out << -1;
    }

    //printGraph(&graph);
    return 0;
}

void findShortestPath(Node *starting) {
    starting->dist = 0;
    set<pair<int, Node *>> nodeSet;

    nodeSet.insert(make_pair(starting->dist, starting));
    Node *tmpNode;
    while (!nodeSet.empty()) {
        tmpNode = (*nodeSet.begin()).second;
        nodeSet.erase(nodeSet.begin());

        for (auto &vic : tmpNode->vic) {
            int alt;
            if (vic.f >= tmpNode->dist) {
                // in case have to wait first time laser turning off
                alt = vic.f + vic.w;
            } else {
                int m = (tmpNode->dist - vic.f) % (vic.y + vic.n);
                if (vic.w + m > vic.y) {
                    // assure you have time to pass right now
                    alt = tmpNode->dist + (vic.y + vic.n) - m + vic.w;
                } else {
                    // else pass next time
                    alt = tmpNode->dist + vic.w;
                }
            }

            if (alt < vic.node->dist) {
                if (vic.node->dist != INT32_MAX) {
                    nodeSet.erase(nodeSet.find(make_pair(vic.node->dist, vic.node)));
                }
                vic.node->prev = tmpNode;
                vic.node->dist = alt;
                nodeSet.insert(make_pair(alt, vic.node));
            }
            //printSet(&nodeSet);
        }
    }
}

void printSet(set<pair<int, Node *>> *mySet) {
    for (auto &myPair : *mySet) {
        out << "V: " << myPair.second->value << " | D:" << myPair.first << endl;
    }
    out << endl;
}

void printGraph(vector<Node> *graph) {
    for (auto &node : *graph) {
        out << "N" << node.value << ": " << endl;
        for (auto &vic : node.vic) {
            out << vic.node->value << " | W:" << vic.w << " | F:" << vic.f << " | Y:" << vic.y << " | N:" << vic.n
                << " | Dist:" << node.dist
                << endl;
        }
        out << endl;
    }
    out << endl;
}

void printDistVector(vector<pair<Node *, int>> *distVector) {
    for (auto &node : *distVector) {
        cout << "N:" << node.first->value << "| dist:" << node.first->dist << endl;
    }
}

void printPath(stack<int> *path) {
    while (!path->empty()) {
        out << path->top() << endl;
        path->pop();
    }
}

void getShortestPath(Node *node, stack<int> *path) {
    path->push(node->value);
    if (node->prev != nullptr) {
        getShortestPath(node->prev, path);
    }
}

