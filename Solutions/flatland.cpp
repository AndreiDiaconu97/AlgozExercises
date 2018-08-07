#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <time.h>
#include <cmath>
#include <cstring>

using namespace std;
vector<pair<int, char>> checkOrientation(vector<pair<int, char>> *triangles, unsigned int pos);
void printTriangles(vector<pair<int, char>> *string1);
void copyVector(vector<pair<int, char>> *vectorTo, vector<pair<int, char>> *vectorFrom);
set<int> random_check(vector<pair<int, char>> *actualTriangles);
set<int> recursive_check(vector<pair<int, char>> *actualTriangles);
void rec_check(vector<pair<int, char>> *actualTriangles, set<int> *_solos, string precedent);
void testAll(unsigned int len);
vector<char> copy(vector<char> seq);
vector<vector<pair<int, char>>> allSequencies(unsigned int length);
void insertFullEven(set<int> *_solos, int size);
unsigned int size; // sempre dispari
//unsigned int backupSize;

int backupSize;
bool dInMiddle = false;
ifstream in("input.txt");
ofstream out("output.txt");

int main() {
    in >> size;
    backupSize = size;
    
    vector<pair<int, char>> triangles;
    triangles.resize(size);
    //backupTriangles.resize(size);
    
    pair<int, char> tmp;
    for (int i = 0; i < size; ++i) {
        in >> tmp.second;
        tmp.first = i;
        triangles[i].first = tmp.first;
        triangles[i].second = tmp.second;
        //cout << " " << triangles[i].second << " "; // //cout
    }
    //cout << endl; // //cout
    //copyVector(&backupTriangles, &triangles);

    // TODO(select one of those)
    //cout << "random:    ";
    random_check(&triangles);
    //cout << "\nrecursive: ";
    //recursive_check(&triangles);

    //out.close();
    //testAll(15);
    return 0;
}
void rec_check(vector<pair<int, char>> *actualTriangles, set<int> *_solos, string precedent) {
    if (actualTriangles->size() == 0) {
        dInMiddle = false;
    } else if (actualTriangles->size() == 1) {
        _solos->insert(actualTriangles->front().first);
        dInMiddle = false;
    } else {
        /*
        for (unsigned int i = 1; i < actualTriangles->size() - 1; ++i) { // loop here
            vector<pair<int, char>> newTmpVector;
            newTmpVector.resize(actualTriangles->size());
            copyVector(&newTmpVector, actualTriangles);

            //////////////////////////////// optimizations here
            if (actualTriangles->size() > 5) { // MAIN optimization
                if (actualTriangles->at(1).second == 'd') {
                    if (actualTriangles->at(3).second == 's') {
                        _solos->insert(actualTriangles->at(0).first);
                        newTmpVector.erase(newTmpVector.begin(), newTmpVector.begin() + 4);
                        i = actualTriangles->size(); // exit from for loop and continue with recursion
                    }
                } else {
                    newTmpVector.erase(newTmpVector.begin(), newTmpVector.begin() + 4);
                    i = actualTriangles->size(); // exit from for loop and continue with recursion
                }
            } else if (actualTriangles->size() == 5) { // MUST consider earlier cases
                newTmpVector = checkOrientation(actualTriangles, i);
            } else {
                newTmpVector = checkOrientation(actualTriangles, i);
            }
            */
        // TODO(insertFullEven() function may be not the solution, neither manual inserting from pos. 0||2||4)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (actualTriangles->size() > 5) {
            if (actualTriangles->at(1).second == 'd') {
                if (actualTriangles->at(3).second == 's') { // dds, dss
                    if ((precedent == "sss") || (precedent == "sdd") || (precedent == "sds")) {
                        if (actualTriangles->at(0).second == 's') {
                            insertFullEven(_solos, actualTriangles->at(0).first + 1);
                        } else {
                            _solos->insert(actualTriangles->at(0).first);
                        }
                    } else if ((precedent == "ssd") || (precedent == "dsd") || (precedent == "ddd")) {
                        _solos->insert(actualTriangles->at(4).first);
                    } else {
                        _solos->insert(actualTriangles->at(0).first);
                    }
                    precedent = "dds";
                } else if (actualTriangles->at(2).second == 's') { // dsd
                    if (precedent == "") {
                        _solos->insert(actualTriangles->at(0).first);
                    } else if ((precedent == "sdd") || (precedent == "sds")) {
                        insertFullEven(_solos, backupSize);
                    } else if (precedent == "sss") {
                        if (actualTriangles->at(0).second == 's') {
                            insertFullEven(_solos, actualTriangles->at(0).first + 1);
                        } else {
                            insertFullEven(_solos, actualTriangles->at(0).first + 3);
                        }
                    } else if ((precedent == "dds") || (precedent == "dss")) {
                        if (actualTriangles->at(0).second == 's') {
                            _solos->insert(actualTriangles->at(0).first);
                        } else {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(2).first);
                            //_solos->insert(actualTriangles->at(4).first);
                        }
                    }
                    // if not "dsd","ddd","ssd","ssd" do nothing
                    precedent = "dsd";
                } else { // ddd
                    if (precedent == "") {
                        _solos->insert(actualTriangles->at(0).first);
                    } else if ((precedent == "sds") || (precedent == "sdd") || (precedent == "sss") ||
                               (precedent == "dds")) {
                        if (actualTriangles->at(0).second == 'd') {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(2).first);
                        } else {
                            if ((precedent == "sds") || (precedent == "sdd")) {
                                insertFullEven(_solos, backupSize);
                            } else if (precedent == "sss") {
                                insertFullEven(_solos, actualTriangles->at(0).first + 1);
                            } else if (precedent == "dds") {
                                _solos->insert(actualTriangles->at(0).first);
                            }
                        }
                    }
                    // else do nothing for "ddd","dsd","ssd"
                    precedent = "ddd";
                }
            } else {
                if (actualTriangles->at(2).second == 's') {
                    if (actualTriangles->at(3).second == 's') { // sss
                        precedent = "sss";
                    } else { //ssd TODO(probably work needed here)
                        if (precedent == "") {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(2).first);
                        } else if ((precedent == "sdd") || (precedent == "sds")) {
                            insertFullEven(_solos, backupSize);
                        } else if ((precedent == "ssd") || (precedent == "ddd") || (precedent == "dsd") ||
                                   (precedent == "sdd")) {
                            _solos->insert(actualTriangles->at(2).first);
                        } else if (precedent == "sss") {
                            insertFullEven(_solos, actualTriangles->at(0).first + 3);
                        } else if (precedent == "dds") {
                            if (actualTriangles->at(0).second == 's') {
                                _solos->insert(actualTriangles->at(0).first);
                                _solos->insert(actualTriangles->at(2).first);
                            } else {
                                _solos->insert(actualTriangles->at(0).first);
                                _solos->insert(actualTriangles->at(2).first);
                                //_solos->insert(actualTriangles->at(4).first);
                            }
                        }
                        precedent = "ssd";
                    }
                } else {
                    if (actualTriangles->at(3).second == 's') { // sds
                        if (precedent == "ssd") {
                            //_solos->insert(actualTriangles->at(2).first);
                        } else if (precedent == "dds") {
                            _solos->insert(actualTriangles->at(2).first);
                        }
                        precedent = "sds";
                    } else { // sdd

                        _solos->insert(actualTriangles->at(2).first);
                        precedent = "sdd";
                    }
                }
            }
            if (actualTriangles->at(4).second == 'd') {
                dInMiddle = true;
            }
            actualTriangles->erase(actualTriangles->begin(), actualTriangles->begin() + 4);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } else if (actualTriangles->size() == 5) { // MUST consider earlier cases
            if (actualTriangles->at(1).second == 'd') {
                if (actualTriangles->at(3).second == 's') { // dds, dss
                    if ((precedent == "sss") || (precedent == "sdd") || (precedent == "sds")) {
                        if (actualTriangles->at(0).second == 's') {
                            insertFullEven(_solos, backupSize - 4);
                            _solos->insert(actualTriangles->at(4).first);
                        } else {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(4).first);
                        }
                    } else if ((precedent == "ssd") || (precedent == "dsd") || (precedent == "ddd")) {
                        _solos->insert(actualTriangles->at(4).first);
                        if (precedent == "ssd") {
                            _solos->insert(actualTriangles->at(4).first);
                        }
                    } else {
                        _solos->insert(actualTriangles->at(0).first);
                        _solos->insert(actualTriangles->at(4).first);
                    }
                } else if (actualTriangles->at(2).second == 's') { // dsd
                    if (precedent == "") {
                        _solos->insert(actualTriangles->at(0).first);
                    } else if ((precedent == "sdd") || (precedent == "sds")) {
                        insertFullEven(_solos, backupSize);
                    } else if (precedent == "sss") {
                        if (dInMiddle) {
                            insertFullEven(_solos, backupSize);
                        } else {
                            insertFullEven(_solos, backupSize - 4);
                        }
                    } else if ((precedent == "dds") || (precedent == "dss")) {
                        if (actualTriangles->at(0).second == 's') {
                            _solos->insert(actualTriangles->at(0).first);
                        } else {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(2).first);
                            _solos->insert(actualTriangles->at(4).first);
                        }
                    } else {// if not "dsd","ddd","ssd"
                        // do nothing
                    }
                } else { // ddd
                    if (precedent == "") {
                        _solos->insert(actualTriangles->at(0).first);
                    } else if ((precedent == "sds") || (precedent == "sdd") || (precedent == "sss") ||
                               (precedent == "dds")) {
                        if (actualTriangles->at(0).second == 'd') {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(2).first);
                            _solos->insert(actualTriangles->at(4).first);
                        } else {
                            if ((precedent == "sds") || (precedent == "sdd")) {
                                insertFullEven(_solos, backupSize);
                            } else if (precedent == "sss") {
                                if (dInMiddle) {
                                    insertFullEven(_solos, backupSize);
                                } else {
                                    insertFullEven(_solos, backupSize - 4);
                                }
                            } else if (precedent == "dds") {
                                _solos->insert(actualTriangles->at(0).first);
                            }
                        }
                    }
                    // else do nothing for "ddd","dsd","ssd"
                }
            } else {
                if (actualTriangles->at(2).second == 's') {
                    if (actualTriangles->at(3).second == 's') { // sss
                        _solos->insert(actualTriangles->at(4).first);
                    } else { //ssd TODO(probably work needed here)
                        if (precedent == "") {
                            _solos->insert(actualTriangles->at(0).first);
                            _solos->insert(actualTriangles->at(2).first);
                        } else if ((precedent == "sdd") || (precedent == "sds")) {
                            insertFullEven(_solos, backupSize);
                        } else if ((precedent == "ssd") || (precedent == "ddd") || (precedent == "dsd")) {
                            _solos->insert(actualTriangles->at(2).first);
                        } else if (precedent == "sss") {
                            if (dInMiddle) {
                                insertFullEven(_solos, backupSize);
                            } else {
                                insertFullEven(_solos, backupSize - 2);
                            }
                            //insertFullEven(_solos, backupSize);
                        } else if (precedent == "dds") {
                            if (actualTriangles->at(0).second == 's') {
                                _solos->insert(actualTriangles->at(0).first);
                                _solos->insert(actualTriangles->at(2).first);
                            } else {
                                _solos->insert(actualTriangles->at(0).first);
                                _solos->insert(actualTriangles->at(2).first);
                                _solos->insert(actualTriangles->at(4).first);
                            }
                        }
                    }
                } else {
                    if (actualTriangles->at(3).second == 's') { // sds
                        _solos->insert(actualTriangles->at(4).first);
                    } else { // sdd
                        _solos->insert(actualTriangles->at(2).first);
                        _solos->insert(actualTriangles->at(4).first);
                    }
                }
            }
            actualTriangles->clear(); // actualTriangles->size()==5 case is an exit one
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } else { // actualTriangles->size()==3
            if (precedent == "dds") {
                if ((actualTriangles->at(0).second == 'd') && (actualTriangles->at(1).second == 'd')) {
                    _solos->insert(actualTriangles->at(2).first);
                } else if ((actualTriangles->at(0).second == 's') && (actualTriangles->at(1).second == 'd')) {
                    if (dInMiddle) {
                        insertFullEven(_solos, backupSize);
                    }
                }
            } else if (precedent == "ddd") {
                if (dInMiddle) {
                    if (actualTriangles->at(1).second == 'd') {
                        if (actualTriangles->at(0).second == 's') {
                            insertFullEven(_solos, backupSize);
                        } else {
                            actualTriangles->clear();
                        }
                    }
                } else {
                    if (actualTriangles->at(1).second == 'd') {
                        actualTriangles->clear();
                    }
                }
            } else if (precedent == "dsd") {
                if (actualTriangles->at(1).second == 'd') {
                    if (actualTriangles->at(0).second == 's') {
                        if (dInMiddle) {
                            insertFullEven(_solos, backupSize);
                        } else {
                            actualTriangles->clear();
                        }
                    } else {
                        actualTriangles->clear();
                    }
                }
            } else if (precedent == "sds") {
                if ((actualTriangles->at(0).second == 's') && (actualTriangles->at(1).second == 'd')) {
                    insertFullEven(_solos, backupSize);
                } else if ((actualTriangles->at(0).second == 'd') && (actualTriangles->at(1).second == 'd')) {
                    _solos->insert(actualTriangles->at(2).first);
                }
            } else if (precedent == "sdd") {
                if ((actualTriangles->at(0).second == 's') && (actualTriangles->at(1).second == 'd')) {
                    insertFullEven(_solos, backupSize);
                } else if ((actualTriangles->at(0).second == 'd') && (actualTriangles->at(1).second == 'd')) {
                    _solos->insert(actualTriangles->at(2).first);
                }
            } else if (precedent == "ssd") {
                if ((actualTriangles->at(0).second == 's') && (actualTriangles->at(1).second == 's')) {
                    _solos->insert(actualTriangles->at(2).first);
                } else if ((actualTriangles->at(0).second == 's') && (actualTriangles->at(1).second == 'd')) {
                    if (dInMiddle) {
                        insertFullEven(_solos, backupSize);
                    } else {
                        actualTriangles->clear();
                    }
                } else if ((actualTriangles->at(0).second == 'd') && (actualTriangles->at(1).second == 'd')) {
                    actualTriangles->clear();
                }
            } else if (precedent == "sss") {
                if ((actualTriangles->at(0).second == 's') && (actualTriangles->at(1).second == 'd')) {
                    if (dInMiddle) {
                        insertFullEven(_solos, backupSize - 2);
                    } else {
                        insertFullEven(_solos, backupSize - 2);
                    }
                } else if ((actualTriangles->at(0).second == 'd') && (actualTriangles->at(1).second == 'd')) {
                    _solos->insert(actualTriangles->at(2).first);
                }
            }
            if (!actualTriangles->empty()) {
                *actualTriangles = checkOrientation(actualTriangles, 1);
            }
        }
        //////////////////////////////// end of optimizations
        rec_check(actualTriangles, _solos, precedent);
    }
}
void insertFullEven(set<int> *_solos, int size) {
    for (int i = 0; i < size; i += 2) {
        _solos->insert(i);
    }
}

set<int> recursive_check(vector<pair<int, char>> *actualTriangles) {
    vector<pair<int, char>> backupTriangles;
    backupTriangles.resize(actualTriangles->size());
    copyVector(&backupTriangles, actualTriangles);
    set<int> solosTmp;
    clock_t initial = clock();
    rec_check(actualTriangles, &solosTmp, "");
    int time = clock() - initial;
    for (unsigned int j = 0; j < actualTriangles->size(); ++j) {
        if (solosTmp.find(actualTriangles->at(j).first) != solosTmp.end()) {
            //cout << "(" << actualTriangles->at(j).first << ")";
        } else {
            //cout << " " << actualTriangles->at(j).first << " ";
        }
    }
    //cout << " --> Recursive: " << time << "ms" << endl;
    // printing value in output file
    //out << solosTmp.size() << endl;
    //cout << "Recursive solutions = ";
    for (int solo : solosTmp) {
        //out << solo << " ";
        //cout << solo << " ";
    }
    actualTriangles->resize(backupTriangles.size());
    copyVector(actualTriangles, &backupTriangles);
    //cout << endl;
    return solosTmp;
}
set<int> random_check(vector<pair<int, char>> *actualTriangles) {
    vector<pair<int, char>> backupTriangles;
    backupTriangles.resize(actualTriangles->size());
    copyVector(&backupTriangles, actualTriangles);

    set<int> solos;
    clock_t initial = clock();
    unsigned int rndm;
    srand(time(NULL));
    for (int j = 0; j < 16300; ++j) {
        while (actualTriangles->size() > 1) {
            rndm = rand() % (actualTriangles->size() - 2) + 1;
            *actualTriangles = checkOrientation(actualTriangles, rndm);
        }
        solos.insert(actualTriangles->front().first);
        actualTriangles->resize(backupTriangles.size());
        copyVector(actualTriangles, &backupTriangles);
    }
    // printing value in input file
    for (unsigned int j = 0; j < actualTriangles->size(); ++j) {
        if (solos.find(actualTriangles->at(j).first) != solos.end()) {
            //cout << "(" << actualTriangles->at(j).first << ")";
        } else {
            //cout << " " << actualTriangles->at(j).first << " ";
        }
    }
    //cout << " --> Random:    " << clock() - initial << "ms" << endl;
    //cout << "Random solutions    = ";
    out << solos.size() << endl;
    for (int solo : solos) {
        out << solo << " ";
    }
    //cout << endl;
    return solos;
}

void copyVector(vector<pair<int, char>> *vectorTo, vector<pair<int, char>> *vectorFrom) {
    for (unsigned int i = 0; i < vectorTo->size(); ++i) {
        vectorTo->at(i).first = vectorFrom->at(i).first;
        vectorTo->at(i).second = vectorFrom->at(i).second;
    }
}

void printTriangles(vector<pair<int, char>> *triangles) {
    for (auto &triangle : *triangles) {
        //cout << "index: " << triangle.first << " value: " << triangle.second << endl;
    }
    //cout << endl;
}

vector<pair<int, char>> checkOrientation(vector<pair<int, char>> *triangles, unsigned int pos) {
    //size -= 2;
    vector<pair<int, char>> lesserTriangles;
    lesserTriangles.resize(triangles->size() - 2);

    if (triangles->at(pos).second == 's') {
        ////cout << pos - 1 << "<-" << pos << " (relative index)" << endl;
        for (unsigned int i = 0; i < pos - 1; ++i) {
            lesserTriangles[i].second = triangles->at(i).second;
            lesserTriangles[i].first = triangles->at(i).first;
        }
        for (unsigned int j = pos + 1; j < triangles->size(); ++j) {
            lesserTriangles[j - 2].second = triangles->at(j).second;
            lesserTriangles[j - 2].first = triangles->at(j).first;
        }
    } else {

        ////cout << pos << "->" << pos + 1 << " (relative index)" << endl;
        for (unsigned int i = 0; i < pos; ++i) {
            lesserTriangles[i].second = triangles->at(i).second;
            lesserTriangles[i].first = triangles->at(i).first;
        }
        for (unsigned int j = pos + 2; j < triangles->size(); ++j) {
            lesserTriangles[j - 2].second = triangles->at(j).second;
            lesserTriangles[j - 2].first = triangles->at(j).first;
        }
    }
    return lesserTriangles;
}


vector<vector<pair<int, char>>> allSequencies(unsigned int length) {
    vector<vector<char>> result;
    result.resize(static_cast<unsigned int>(pow(2, length)));

    result[0].push_back('s');
    result[1].push_back('d');

    for (int i = 1; i < length; i++) {
        int currentLength = static_cast<int>(pow(2, i));
        for (int k = 0; k < currentLength; k++) {
            vector<char> added = copy(result[k]);
            result[k].insert(result[k].begin(), 's');
            added.insert(added.begin(), 'd');

            result[currentLength + k] = added;
        }
    }

    vector<vector<pair<int, char>>> resultFinal;
    resultFinal.resize(result.size());
    for (int i = 0; i < resultFinal.size(); i++) {
        resultFinal[i].resize(length);
        for (int k = 0; k < length; k++) {
            resultFinal[i][k].first = k;
            resultFinal[i][k].second = result[i][k];
        }
    }

    return resultFinal;
}

vector<char> copy(vector<char> seq) {
    vector<char> result;
    result.resize(seq.size());
    for (int i = 0; i < seq.size(); i++) {
        result[i] = seq[i];
    }
    return result;
}

void testAll(unsigned int len) {
    vector<vector<pair<int, char>>> sequences = allSequencies(len);
    ofstream output("output.txt");
    int l = 1;
    int err = 0;
    for (vector<pair<int, char>> seq : sequences) {
        backupSize = seq.size();
        clock_t begin1 = clock();
        set<int> resultOld = random_check(&seq);
        clock_t end1 = clock();
        set<int> result = recursive_check(&seq);
        clock_t end2 = clock();
        for (int i = 0; i < seq.size(); i++) {
            //cout<<seq[i].first;
            if (result.find(i) != result.end()) {
                output << "(" << seq[i].second << ")";
            } else {
                output << " " << seq[i].second << " ";
            }
        }
        output << " | ";
        for (int i = 0; i < seq.size(); i++) {
            //cout<<seq[i].first;
            if (resultOld.find(i) != resultOld.end()) {
                output << "(" << seq[i].second << ")";
            } else {
                output << " " << seq[i].second << " ";
            }
        }
        //for(int i : resultOld){//cout<<i<<" ";}//cout<<endl;for(int i: result){//cout<<i<<" ";}
        output << "   New: " << end2 - end1 << "    Old: " << end1 - begin1;

        if (result != resultOld) {
            cout << "error line: " << l << endl << endl;
            output << " - ERROR! " << "- Errors: " << ++err << "/" << pow(2, len);
        }
        output << endl;
        l++;
    }
}
