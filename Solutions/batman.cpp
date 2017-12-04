#include <iostream>
#include <vector>
#include <fstream>
#include <stack>

using namespace std;

struct Node {
    // standard
    unsigned int value = 0;
    int depth = 0;
    vector<Node *> vic;
    vector<Node *> fathers;
    bool isVisited = false;

    // Tarjan
    int lowkey = 0;
    bool onStack = false;
    
    // main algorithm
    int rightPaths = 0;

    // merging nodes for each component
    Node *representative = nullptr;
};

void printGraph(vector<Node> *graph);

void findStrongComps(Node *node, vector<Node> *graph, stack<int> *strongStack);

void batCombinations(Node *node, unsigned int target);

void printStack(stack<int> tmpStack);

void resetVisits(vector<Node> *graph);

void printNodeProperties(vector<Node> *graph);

vector<Node> *shrinkGraph(Node *node, unsigned int size);

void insertMergedNodes(vector<Node> *newGraph, Node *node, Node *father, int actualComponentID);

int globalDepth = 0;

ifstream in("input.txt");
ofstream out("output.txt");

int main() {
    vector<Node> graph;
    stack<int> strongStack;
    unsigned int N, M, source, destination;
    in >> N >> M >> source >> destination;
    graph.resize(N);
    int from, to;
    for (int i = 0; i < M; ++i) {
        in >> from >> to;
        // pushing nodes
        graph[from].vic.push_back(&graph[to]);
        graph[to].fathers.push_back(&graph[from]);
    }
    // inserting values
    for (unsigned int k = 0; k < N; ++k) {
        graph.at(k).value = k;
    }
    // finding strongly connected components >= 3
    for (auto &node : graph) {
        if (!node.isVisited) {
            findStrongComps(&node, &graph, &strongStack);
        }
    }
    // final nodes are not necessarily the original target and destination
    source = graph.at(source).representative->value;
    destination = graph.at(destination).representative->value;

    resetVisits(&graph);
    vector<Node> *newGraph;
    newGraph = shrinkGraph(&graph.at(source), N);
    batCombinations(&newGraph->at(source), destination);
    out << newGraph->at(source).rightPaths << endl;
    //printGraph(newGraph);
    //printNodeProperties(newGraph);
    return 0;
}

vector<Node> *shrinkGraph(Node *node, unsigned int size) {
    auto *newGraph = new vector<Node>;
    newGraph->resize(size);
    for (unsigned int i = 0; i < newGraph->size(); ++i) {
        newGraph->at(i).value = i;
    }
    insertMergedNodes(newGraph, node, node, -1);
    return newGraph;
}

void insertMergedNodes(vector<Node> *newGraph, Node *node, Node *father, int actualComponentID) {
    node->isVisited = true;
    //cout << node->value << ": ";

    // changing component
    if (node->representative->value != actualComponentID) {
        actualComponentID = node->representative->value;
        father = node;
    }

    // general enemy insertion
    for (auto &vic : node->vic) {
        //cout << vic->value << " ID: " <<vic->strongCompId << " | ";
        if (vic->representative->value != node->representative->value) {
            newGraph->at(node->representative->value).vic.push_back(&newGraph->at(vic->representative->value));
        }
    }
    //cout << endl;

    // RECURSION
    for (auto &vic : node->vic) {
        if (!vic->isVisited) {
            insertMergedNodes(newGraph, vic, father, actualComponentID);
        }
    }
}


void batCombinations(Node *node, unsigned int target) {
    //cout << "N: " << node->value << endl;
    node->isVisited = true;
    for (auto &vic : node->vic) {
        if (vic->value == target) {
            node->rightPaths++;
        } else if (!vic->isVisited) {
            batCombinations(vic, target);
        }
        node->rightPaths += vic->rightPaths;
    }
}

void findStrongComps(Node *node, vector<Node> *graph, stack<int> *strongStack) {
    node->depth = globalDepth;
    node->lowkey = globalDepth;
    node->isVisited = true;
    globalDepth++;

    strongStack->push(node->value);
    node->onStack = true;
    //printStack(*strongStack);

    for (auto &vic : node->vic) {
        if (!vic->isVisited) {
            findStrongComps(vic, graph, strongStack);
            //node->vicNum++;
            node->lowkey = min(node->lowkey, vic->lowkey);
        } else if (vic->onStack) {
            node->lowkey = min(node->lowkey, vic->depth);
        }
    }

    if (node->lowkey == node->depth) {
        unsigned int tmpNodeValue;
        Node *tmpNode;
        Node *repr = &graph->at(strongStack->top());
        int counter = 0;
        vector<Node *> tmpConnVector;
        do {
            tmpNodeValue = strongStack->top();
            //cout << strongStack->top() << ", ";
            strongStack->pop();
            tmpNode = &graph->at(tmpNodeValue);
            tmpNode->onStack = false;
            tmpNode->representative = repr;
            tmpConnVector.push_back(tmpNode);
            counter++;
        } while (node->value != tmpNodeValue);
    }
}

void printStack(stack<int> tmpStack) {
    while (!tmpStack.empty()) {
        out << tmpStack.top() << " ";
        tmpStack.pop();
    }
    out << endl;
} // OK

void printGraph(vector<Node> *graph) {
    for (auto &node : *graph) {
        out << node.value << " VIC: ";
        for (auto &vic : node.vic) {
            out << vic->value << " ";
        }
        out << " | F: ";
        for (auto &father : node.fathers) {
            out << father->value << " ";
        }
        if (node.representative != nullptr) {
            out << "REP: " << node.representative->value;
        }
        out << endl;
    }
    out << endl;
} // OK

void printNodeProperties(vector<Node> *graph) {
    for (auto &node : *graph) {
        out << node.value << " | rightPaths: " << node.rightPaths << endl;
    }
}

void resetVisits(vector<Node> *graph) {
    for (auto &node : *graph) {
        node.isVisited = false;
    }
}
