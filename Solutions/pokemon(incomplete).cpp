#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cmath>

using namespace std;

struct Edge {
    int color = 0;
    int first = 0;
    int second = 0;
};

struct Node {
    set<Node *> vic;
    unsigned int value = 0;
    bool isDeleted = false;
    bool isVisited = false;
    int childCount = 0;
    int vicNum = 0;

    int depth = 0;
    int lowkey = 0;
    bool tmpArticulation = false;
    bool isArticulation = false;
    bool usedForPathing = false;

    vector<Edge *> edgePointers;
};

int largestPrime(int a);

set<int> *primeFactors(int n);

int gcd(int a, int b);

void printGraph(vector<Node> *graph);

void calculateSpeciesNum(vector<Node> *graph);

void pathLength(Node *current, Node *initial, bool isStarting, int depth);

void resetVisits(vector<Node> *graph);

void removeClaws(vector<Node> *graph);

void findingOptimalSpeciesNum(vector<Node> *graph);

void pathLengthWithTwo(Node *current, Node *initial, bool isStarting, int depth);

void removeClawsRec(Node *node, vector<Node> *graph);

void maxVertexGrade(vector<Node> *graph);

void saveBidirectedGraph(vector<Node> *graph);

void initializeGraphs(vector<vector<Node>> *graphs);

void tarjan(Node *node, Node *father, int depth);

void calculatePathLen(Node *current, Node *initial, bool isStarting, int depth, int expecting);

void resetUsedForPathing(vector<Node> *graph);

set<int> pathSizes;
int species = 1;
int maxGrade = 0;
ifstream in("input.txt");
ofstream out("output.txt");

int main() {
    // testing all inputs
    /*
    vector<vector<Node>> graphs;
    initializeGraphs(&graphs);
    */


    // TODO use following code - main program

    vector<Node> graph;
    unsigned int M;
    unsigned int N;
    in >> N >> M;
    graph.resize(N);
    unsigned int from, to;

    for (int i = 0; i < M; ++i) {
        in >> from >> to;
        // pushing nodes
        graph[from].vicNum++;
        graph[to].vicNum++;

        graph[from].vic.insert(&graph[to]);
        graph[to].vic.insert(&graph[from]);

        auto *tmpEdge = new Edge();
        tmpEdge->first = from;
        tmpEdge->second = to;
        graph[from].edgePointers.push_back(tmpEdge);
        graph[to].edgePointers.push_back(tmpEdge);
    }
    for (unsigned int k = 0; k < N; ++k) {
        graph.at(k).value = k;
    }
    //printGraph(&graph);

    tarjan(&graph.at(0), &graph.at(0), 0);
    removeClaws(&graph);
    maxVertexGrade(&graph);
    calculateSpeciesNum(&graph);
    findingOptimalSpeciesNum(&graph);
    //printGraph(&graph);
    //out << "species: ";
    out << species;
    //saveBidirectedGraph(&graph);

    return 0;
}

void initializeGraphs(vector<vector<Node>> *graphs) {
    graphs->resize(20);

    vector<int> solutions;
    solutions.push_back(4);
    solutions.push_back(250);
    solutions.push_back(110);
    solutions.push_back(10);
    solutions.push_back(8);
    solutions.push_back(10);
    solutions.push_back(4);
    solutions.push_back(3);
    solutions.push_back(10);
    solutions.push_back(13);
    solutions.push_back(8);
    solutions.push_back(70);
    solutions.push_back(3);
    solutions.push_back(17);
    solutions.push_back(1);
    solutions.push_back(2);
    solutions.push_back(5);
    solutions.push_back(7);
    solutions.push_back(1);
    solutions.push_back(3);

    for (unsigned int i = 0; i < 20; ++i) {
        ifstream tmpGraph("input" + to_string(i) + ".txt");

        unsigned int M;
        unsigned int N;
        tmpGraph >> N >> M;
        graphs->at(i).resize(N);
        unsigned int from, to;

        for (int m = 0; m < M; ++m) {
            tmpGraph >> from >> to;
            // pushing nodes
            graphs->at(i).at(from).vicNum++;
            graphs->at(i).at(to).vicNum++;

            graphs->at(i).at(from).vic.insert(&(graphs->at(i).at(to)));
            graphs->at(i).at(to).vic.insert(&(graphs->at(i).at(from)));

            auto *tmpEdge = new Edge();
            tmpEdge->first = from;
            tmpEdge->second = to;
            graphs->at(i).at(from).edgePointers.push_back(tmpEdge);
            graphs->at(i).at(to).edgePointers.push_back(tmpEdge);
        }
        for (unsigned int k = 0; k < N; ++k) {
            graphs->at(i).at(k).value = k;
        }

        out << "GRAPH: " << i << endl;
        //printGraph(&graphs->at(i));

        tarjan(&graphs->at(i).at(0), &graphs->at(i).at(0), 0);
        removeClaws(&graphs->at(i));
        maxVertexGrade(&graphs->at(i));
        // TODO try new algoritm
        calculateSpeciesNum(&graphs->at(i));
        findingOptimalSpeciesNum(&graphs->at(i));

        cout << i << " S:" << species << " - Exp:" << solutions.at(i) << " | " << (species == solutions[i]) << endl;
        //saveBidirectedGraph(&graphs->at(i));

        pathSizes.clear();
        species = 1;
        maxGrade = 0;
        tmpGraph.close();
    }

}

void findingOptimalSpeciesNum(vector<Node> *graph) {
    int tmp = *pathSizes.begin();
    cout << tmp << endl;
    for (auto &path : pathSizes) {
        //cout << path << endl;
        tmp = gcd(tmp, path);
    }
    species = tmp;
} //OK

void calculateSpeciesNum(vector<Node> *graph) {
    resetVisits(graph);
    // case there are only grade-2 nodes
    //TODO try new algorithm
    /*
    if (maxGrade == 2) {
        for (auto &node : *graph) {
            if ((!node.isVisited) && (node.vicNum == 2)) {
                pathLengthWithTwo(&node, &node, true, 0);
            }
        }
        resetVisits(graph);
    }

    for (auto &node : *graph) {
        //cout << node.value << endl;
        if (node.vicNum > 2) {
            pathLength(&node, &node, true, 0);
            //out << node.value << endl;

        }
        resetVisits(graph);
    }
    */

    if (maxGrade == 2) {
        for (auto &node : *graph) {
            if ((!node.isVisited) && (node.vicNum == 2)) {
                pathLengthWithTwo(&node, &node, true, 0);
                resetVisits(graph);
            }
        }
    } else {

        for (auto &node : *graph) {
            if (node.vicNum > 2) {
                // per tutti i nodi calcolo cammino fino a nodi validi
                calculatePathLen(&node, &node, true, 0, 0);
                resetUsedForPathing(graph);
                //resetVisits(graph);
            }
        }
    }


    /*
    cout << "PathSizes:" << endl;
    for (auto &value : pathSizes) {
        cout << value << ",";
    }
    cout << endl;
    */
}

void resetUsedForPathing(vector<Node> *graph) {
    for (auto &node : *graph) {
        node.usedForPathing = false;
    }
}

void calculatePathLen(Node *current, Node *initial, bool isStarting, int depth, int expecting) {
    current->isVisited = true;
    depth++;

    if (current->value == initial->value) {
        if (isStarting) {
            // FIRST
            //out << "first:" << current->value << " D:" << depth << " L:" << current->lowkey << " Exp." << expecting;
            //out << endl;
            for (auto &vic : current->vic) {
                if ((!vic->isDeleted) && (!vic->isVisited)) {
                    // begin following path with a fixed lowkey value
                    calculatePathLen(vic, initial, true, depth, vic->lowkey);
                }
            }
        } else {
            // LAST
            //out << "WRONG last: " << depth << endl;
            //pathSizes.insert(depth);
        }
    } else {
        if (isStarting) {
            // SECOND
            //out << "second:" << current->value << " D:" << depth << " L:" << current->lowkey << " Exp." << expecting;
            //out << endl;
            for (auto &vic : current->vic) {
                if ((!vic->isDeleted) && (vic->value != initial->value)) {
                    if ((!vic->isVisited) && (vic->lowkey == expecting)) {
                        calculatePathLen(vic, initial, false, depth, expecting);
                    } else if ((vic->lowkey != expecting) && (vic->vicNum > 2)) {
                        calculatePathLen(vic, initial, false, depth, expecting);
                    }
                }
            }

        } else {
            // GENERAL
            //out << "general:" << current->value << " D:" << depth << " L:" << current->lowkey << " Exp." << expecting;
            //out << endl;
            for (auto &vic : current->vic) {
                if (!vic->isDeleted) {
                    if ((!vic->isVisited) && (vic->lowkey == expecting)) {
                        calculatePathLen(vic, initial, false, depth, expecting);
                    } else if ((vic->lowkey != expecting) && (vic->vicNum > 2) && (!vic->usedForPathing)) {
                        // CHECK THIS CASE
                        vic->usedForPathing = true;

                        int i = 0;
                        for (auto &vic2 : vic->vic) {
                            if (i == vic->vicNum - 1) {
                                pathSizes.insert(depth);
                                break;
                            }
                            if (vic2->lowkey == expecting) {
                                calculatePathLen(vic, initial, false, depth, expecting);
                                break;
                            }
                            i++;
                        }

                        //calculatePathLen(vic, initial, false, depth, expecting);
                    } else if (vic->value == initial->value) {
                        //out << "end1 D:" << depth << endl;
                        pathSizes.insert(depth);
                    }

                }
            }
        }
    }


}

void tarjan(Node *node, Node *father, int depth) {
    node->depth = depth;
    node->lowkey = depth;
    node->isVisited = true;

    for (auto &vicino : node->vic) {
        if (!vicino->isVisited) {
            //cout << "visit " << vicino->value << endl;
            tarjan(vicino, node, depth + 1);
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
} //OK

void pathLengthWithTwo(Node *current, Node *initial, bool isStarting, int depth) {
    current->isVisited = true;
    depth++;

    for (auto &vic : current->vic) {
        if (!vic->isVisited && vic->vicNum == 2) {
            pathLength(vic, initial, true, depth);
        }
    }
}

void pathLength(Node *current, Node *initial, bool isStarting, int depth) {
    current->isVisited = true;
    depth++;

    if (isStarting) {
        if (current->value == initial->value) {
            // first
            //out << current->value << " first" << endl;
            for (auto &vic : current->vic) {
                if (!vic->isVisited && vic->vicNum == 2) {
                    pathLength(vic, initial, true, depth);
                }
            }

        } else {
            // second
            //out << current->value << " second" << endl;
            for (auto &vic : current->vic) {
                if (!vic->isVisited && vic->vicNum == 2) {
                    pathLength(vic, initial, false, depth);
                } else if ((vic->vicNum > 2) && (vic->value != initial->value)) {
                    // return len
                    //out << initial->value << "->" << current->value << " D:" << depth << " end1" << endl;
                    pathSizes.insert(depth);
                }
            }
        }
    } else {
        if (current->value == initial->value) {
            // last
            //out << current->value << " last" << endl;


        } else {
            // general
            //out << current->value << " general" << endl;
            for (auto &vic : current->vic) {
                if (!vic->isVisited && vic->vicNum == 2) {
                    pathLength(vic, initial, false, depth);
                } else if ((vic->vicNum > 2) && (vic->value != initial->value)) {
                    //out << initial->value << "->" << current->value << " D:" << depth << " end2" << endl;
                    pathSizes.insert(depth);

                    // return len
                } else if (vic->value == initial->value) {
                    //out << initial->value << "->" << vic->value << " D:" << depth << " end3" << endl;
                    pathSizes.insert(depth);
                }
            }
        }
    }

}

void removeClaws(vector<Node> *graph) {
    for (auto &node : *graph) {
        if (node.vicNum == 1) {
            node.vicNum--;
            node.isDeleted = true;
            for (auto &vic : node.vic) {
                graph->at(vic->value).vicNum--;
                removeClawsRec(&graph->at(vic->value), graph);
            }
        }
    }
} //OK

void removeClawsRec(Node *node, vector<Node> *graph) { //OK
    if (node->vicNum == 1) {
        node->vicNum--;
        node->isDeleted = true;
        for (auto &vic : node->vic) {
            if (!vic->isDeleted) {
                graph->at(vic->value).vicNum--;
                removeClawsRec(&graph->at(vic->value), graph);
            }
        }
    }
}

void maxVertexGrade(vector<Node> *graph) {
    for (auto &node : *graph) {
        maxGrade = max(maxGrade, node.vicNum);
    }
} //OK

void resetVisits(vector<Node> *graph) { //OK
    for (auto &node : *graph) {
        node.isVisited = false;
    }
}

void printGraph(vector<Node> *graph) {
    for (auto &node : *graph) {
        out << "N:" << node.value << endl;
        out << "isDeleted: " << node.isDeleted << endl;
        out << "neighbours: " << node.vicNum << endl;
        for (auto &vic : node.vic) {
            if (!vic->isDeleted) {
                out << vic->value << ",";
            }
        }
        out << endl << endl;
    }

} //OK

void saveBidirectedGraph(vector<Node> *graph) {
    for (auto &node : *graph) {
        if (!node.isDeleted) {
            for (auto &vic : node.vic) {
                if (!vic->isDeleted) {
                    out << node.value << " " << vic->value << " " << node.lowkey << endl;
                }
            }
        }
    }
} //TMP

int gcd(int a, int b) {
    int c;
    while (a != 0) {
        c = a;
        a = b % a;
        b = c;
    }
    return b;
} //OK

set<int> *primeFactors(int n) {
    auto *factors = new set<int>;
    while (n % 2 == 0) {
        //cout << 2 << " ";
        factors->insert(2);
        n = n / 2;
    }
    for (int i = 3; i <= sqrt(n); i = i + 2) {
        while (n % i == 0) {
            //cout << i << " ";
            factors->insert(i);
            n = n / i;
        }
    }
    if (n > 2) {
        //cout << n << " ";
        factors->insert(n);
    }
    return factors;
} //DONT NEED

int largestPrime(int a) {
    int large = 0;
    for (int i = 2; a != 1; ++i) {
        while (a % i == 0) {
            a /= i;
            large = i;
        }
    }
    return large;
} //DONT NEED


