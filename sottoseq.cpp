#include <vector>
#include <fstream>

using namespace std;

int main() {
    unsigned int size;
    //vector<int> vect;
    ifstream in("input.txt");
    in >> size;
    //vect.resize(size);
    // getting vector
    int inValue;
    int tmp = 0;
    int tmpMax = 0;

    for (int i = 0; i < size; ++i) {
        in >> inValue;
        tmp += inValue;
        if (tmp < 0) {
            tmp = 0;
        }
        tmpMax = max(tmpMax, tmp);
    }
    ofstream out("output.txt");
    out << tmpMax;
    return 0;
}
