#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;


int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<pair<int, int>> intervals;
    int N;
    in >> N;

    int first, last;
    pair<int, int> *tmpInterval;
    for (int i = 0; i < N; ++i) {
        in >> first >> last;
        tmpInterval = new pair<int, int>(first, last);
        intervals.push_back(*tmpInterval);
    }
    sort(intervals.begin(), intervals.end());

    int maxUncovered = 0;
    int finalL = 0;
    int finalR = 0;

    int tmpL = 0;
    int tmpR = 0;
    for (unsigned int j = 0; j < intervals.size() - 1; ++j) {
        tmpL = intervals.at(j).second;
        tmpR = intervals.at(j + 1).first;

        if (tmpL > intervals.at(j + 1).second) {
            intervals.at(j + 1).second = tmpL;
        } else if (tmpL < tmpR) {
            if (tmpR - tmpL > maxUncovered) {
                maxUncovered = tmpR - tmpL;
                finalL = tmpL;
                finalR = tmpR;
            }

        }
    }
    out << finalL << " " << finalR;
    return 0;
}
