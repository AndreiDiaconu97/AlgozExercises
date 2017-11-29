#include <vector>
#include <fstream>

using namespace std;

void mergeSort(vector<int> *seq, int left, int right);

void myMerge(vector<int> *seq, int left, int center, int right);

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<int> seq;
    int N;
    in >> N;

    int tmp;
    for (int i = 0; i < N; ++i) {
        in >> tmp;
        seq.push_back(tmp);
    }

    mergeSort(&seq, 0, N - 1);

    for (auto &val : seq) {
        out << val << " ";
    }
    return 0;
}

void mergeSort(vector<int> *seq, int left, int right) {

    if (left < right) {
        int center = (left + right) / 2;
        mergeSort(seq, left, center);
        mergeSort(seq, center + 1, right);

        myMerge(seq, left, center, right);
    }


}

void myMerge(vector<int> *seq, int left, int center, int right) {
    int tLeft = left;
    int tCenter = center + 1;

    vector<int> tmpVector;
    int tmp;
    while ((tLeft <= center) && (tCenter <= right)) {
        if (seq->at(tLeft) <= seq->at(tCenter)) {
            tmp = seq->at(tLeft++);
        } else {
            tmp = seq->at(tCenter++);
        }
        tmpVector.push_back(tmp);
    }

    if (tLeft > center) {
        while (tCenter <= right) {
            tmp = seq->at(tCenter++);
            tmpVector.push_back(tmp);
        }
    } else if (tCenter > right) {
        while (tLeft <= center) {
            tmp = seq->at(tLeft++);
            tmpVector.push_back(tmp);
        }
    }

    for (int i = 0; i < tmpVector.size(); ++i) {
        seq->at(left + i) = tmpVector.at(i);

    }
}
