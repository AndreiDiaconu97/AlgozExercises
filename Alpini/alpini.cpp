#include <fstream>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Node {
    int value = 0;
    //int level = 0;
    //int strongVics = 0;
    int gr = 0;
    int grLvl = 0;
    //int passedFound = 0;

    vector<Node *> vic;

    bool isVisited = false;
    bool isDeleted = false;

    /* tarjan */
    int depth;
    int lowkey;
    int childCount = 0;
    bool tmpArticulation = false;
    bool isArticulation = false;
};

/* utility */
void printGraph(vector<Node> &graph);
void resetGraph(vector<Node> &graph);
void resetVisits(vector<Node> &graph);
void printSolution(unordered_set<int> &solution);
void printSet(unordered_set<Node *> &mySet);
int numVic(Node *n);
void removeLeaves(Node *n);
int groupVics(Node *n);
void dfsGroup(Node *n, int g);

void dfs(Node *n, int &last, unordered_set<Node *> &mySet);
void setVertexes(Node *node, Node *father, int depth);

ifstream in("input.txt");
ofstream out("output.txt");

int main() {
    vector<Node> graph;
    unsigned int N, M;
    in >> N >> M;
    graph.resize(N);

    /* graph initialisation */
    int from, to;
    for (int i = 0; i < M; i++) {
        in >> from >> to;
        graph[from].vic.push_back(&graph[to]);
        graph[to].vic.push_back(&graph[from]);
    }
    for (unsigned int k = 0; k < N; k++) {
        graph[k].value = k;
    }

    /* main algorithm */
    unordered_set<Node *> mySet;
    unordered_set<int> solution;
    Node *tmpNode;
    int maxLevel = -1;

    while (maxLevel != 0) {
        /* reset graph and unordered_set */
        resetGraph(graph);
        //mySet.clear();

        /* remove leaves */
        for (int k = 0; k < N; k++) {
            if (!graph[k].isDeleted) {
                removeLeaves(&graph[k]);
            }
        }

        /* tarjan */
        for (int k = 0; k < N; k++) {
            if (!graph[k].isVisited) {
                setVertexes(&graph[k], &graph[k], 0);
            }
        }
        resetVisits(graph);

        /* get groups */
        for (int k = 0; k < N; k++) {
            if ((!graph[k].isVisited) && (!graph[k].isDeleted)) {
                dfsGroup(&graph[k], k);
            }
        }
        resetVisits(graph);

        /* calcualte groupVics */
        for (Node &n : graph) {
            if (!n.isDeleted) {
                n.grLvl = groupVics(&n);
            }
        }

        tmpNode = &graph[0];
        for (Node &n : graph) {
            if (!n.isDeleted) {
                if (n.grLvl > tmpNode->grLvl) {
                    tmpNode = &n;
                } else if ((n.grLvl == tmpNode->grLvl) && (n.isArticulation)) {
                    tmpNode = &n;
                }
            }
        }
        maxLevel = tmpNode->grLvl;
        if (maxLevel == 0) {
            continue;
        }

        //printGraph(graph);
        //out << "MAX: " << tmpNode->value << " vics: " << maxLevel << endl;
        //printSet(mySet);

        /* remove it from graph */
        tmpNode->isDeleted = true;
        solution.insert(tmpNode->value);
    }

    printSolution(solution);
    return 0;
}

void setVertexes(Node *node, Node *father, int depth) {
    if (node->isDeleted) {
        return;
    }

    node->depth = depth;
    node->lowkey = depth;
    node->isVisited = true;

    for (auto &vicino : node->vic) {
        if (!vicino->isDeleted) {
            if (!vicino->isVisited) {
                //out << "visit " << vicino->value << endl;
                setVertexes(vicino, node, depth + 1);
                node->childCount++;
                if (vicino->lowkey >= node->depth) {
                    node->tmpArticulation = true;
                }
                node->lowkey = min(node->lowkey, vicino->lowkey);
            } else if (vicino != father) {
                node->lowkey = min(node->lowkey, vicino->depth);
                //out << "->" << node->lowkey << endl;
            }
        }
    }
    if (((father != node) && (node->tmpArticulation)) || ((father == node) && (node->childCount > 1))) {
        node->isArticulation = true;
    }
}

void printGraph(vector<Node> &graph) {
    for (auto &node : graph) {
        if (!node.isDeleted) {
            out << "N: " << node.value << "\tdel: " << node.isDeleted << "\tArt: " << node.isArticulation << "\tGrVics: " << groupVics(&node) << "\tGrID: " << node.gr << "\tL: " << node.lowkey << endl;
        }
    }
    out << endl;
}

void resetGraph(vector<Node> &graph) {
    for (auto &node : graph) {
        node.isVisited = false;
        //node.level = 0;
        //node.strongVics = 0;
        node.tmpArticulation = false;
        node.isArticulation = false;
        node.childCount = 0;
        node.gr = 0;
        node.grLvl = 0;
    }
}

void resetVisits(vector<Node> &graph) {
    for (auto &node : graph) {
        node.isVisited = false;
    }
}

void printSolution(unordered_set<int> &solution) {
    out << solution.size() << " ";
    for (int n : solution) {
        out << n << " ";
    }
    out << "#" << endl;
}

void printSet(unordered_set<Node *> &mySet) {
    for (Node *n : mySet) {
        out << n->value << "\tGrVics: " << groupVics(n) << "\tGrID: " << n->gr << endl;
    }
    out << "#" << endl
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

void removeLeaves(Node *n) {
    if (numVic(n) <= 1) {
        n->isDeleted = true;
        for (Node *v : n->vic) {
            if (!v->isDeleted) {
                removeLeaves(v);
            }
        }
    }
}

int groupVics(Node *n) {
    map<int, int> counts;
    int mx = 0;
    int add = 0;

    for (Node *v : n->vic) {
        if (!v->isDeleted) {
            if (v->gr == n->gr) {
                counts[v->gr]++;
            } else if ((!n->isArticulation) && (v->isArticulation)) {
                add++;
            } else if ((n->isArticulation) && (!v->isArticulation)) {
                counts[v->gr]++;
            }
        }
    }
    //cout << "Node: " << n->value << ":" << endl;
    for (auto const &x : counts) {
        mx = max(mx, x.second);
        //cout << x.first << "   " << x.second << endl;
    }
    //cout << "MX: " << mx + add << endl;
    return mx + add;
}

void dfsGroup(Node *n, int g) {
    if (n->isDeleted) {
        return;
    }
    n->isVisited = true;
    n->gr = g;

    if (n->isArticulation) {
        return;
    }

    for (Node *v : n->vic) {
        if (!v->isVisited) {
            if (!v->isArticulation) {
                dfsGroup(v, g);
            } else {
                v->gr = g;
                v->isVisited = true;
            }
        }
    }
}