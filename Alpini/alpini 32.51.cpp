#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct Node {
    int value = 0;
    int level = 0;

    vector<Node *> vic;

    bool isVisited = false;
    bool deactivated = false;
    bool isDeleted = false;
};

struct node_compare {
    bool operator()(const Node *n1, const Node *n2) const {
        return n1->value < n2->value;
    }
};

void printGraph(vector<Node> &graph);
void resetGraph(vector<Node> &graph);
void printSolution(set<int> &solution);
void printSet(set<Node *, node_compare> &mySet);
int numVic(Node *n);
void dfs(Node &n, Node &last, set<Node *, node_compare> &mySet);

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

    set<Node *, node_compare> mySet;
    set<int> solution;
    Node *tmpNode;
    int maxLevel;

    /* repeat dfs() until no nodes with level>1 are found */
    do {
        for (int k = 0; k < N; k++) {
            if ((!graph[k].isVisited) && (!graph[k].isDeleted)) {
                dfs(graph[k], graph[k], mySet);
            }
        }
        /* get biggest level node in set and delete it */
        tmpNode = *mySet.begin();
        for (Node *n : mySet) {
            if (n->level > tmpNode->level) {
                tmpNode = n;
            } else if ((n->level == tmpNode->level) && (numVic(n) > numVic(tmpNode))) {
                tmpNode = n;
            }
        }
        //cout << "MAX: " << tmpNode->value << " L:" << tmpNode->level << endl;
        maxLevel = tmpNode->level;
        /* remove it from graph */
        tmpNode->isDeleted = true;
        solution.insert(tmpNode->value);

        //printSet(mySet);

        /* reset visited, deactivated and level */
        resetGraph(graph);
    } while (maxLevel != 0);

    printSolution(solution);

    /* //reverse iteration //
    for (auto it = mySet.rbegin(); it != mySet.rend(); ++it) {
        out << (*it)->value << " ";
    }
    */

    // cout << (*mySet.rbegin())->value << endl; /* getting last */
    //printGraph(graph);
    return 0;
}

void dfs(Node &n, Node &last, set<Node *, node_compare> &mySet) {
    n.isVisited = true;
    //cout << n.value << endl;

    for (Node *v : n.vic) {
        if ((!n.deactivated) && (!v->isDeleted)) {
            if (!v->isVisited) {
                dfs(*v, n, mySet);
            } else {
                if (v != &last) {
                    v->deactivated = true;
                    v->level++;
                    //cout << v->value << " !" << endl;
                    mySet.insert(v);
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

void resetGraph(vector<Node> &graph) {
    for (auto &node : graph) {
        node.isVisited = false;
        node.deactivated = false;
        node.level = 0;
    }
}

void printSolution(set<int> &solution) {
    out << solution.size() << " ";
    for (int n : solution) {
        out << n << " ";
    }
    out << "#" << endl;
}

void printSet(set<Node *, node_compare> &mySet) {
    for (Node *n : mySet) {
        cout << n->value << " -> " << n->level << endl;
    }
    cout << "#" << endl
         << endl;
}

int numVic(Node *n) {
    int num = 0;
    for (Node *v : n->vic) {
        if (!v->isDeleted) {
            num++;
        }
    }
    return num;
}