#include <vector>
#include <fstream>

using namespace std;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    int n, m;
    in >> n >> m;

    out << n + m;
    return 0;
}
