#include <vector>
#include <fstream>

void maxSubArrayum();

void maxSubMatrixSum();

int maxSubArraySum(int *array, int size);

using namespace std;

int main() {
    maxSubMatrixSum();
    //maxSubArrayum();
    return 0;
}

void maxSubMatrixSum() {
    int y;
    int x;
    int sum;
    int maxSum = 0;
    ifstream in("input.txt");
    in >> y >> x;
    int matrix[y][x];

    // putting data into matrix
    for (int k = 0; k < y; ++k) {
        for (int i = 0; i < x; ++i) {
            in >> matrix[k][i];
        }
    }

    // main algorithm
    for (int left = 0; left < x; left++) {
        int temp[y] = {0};
        for (int right = left; right < x; right++) {
            for (int i = 0; i < y; ++i) {
                temp[i] += matrix[i][right];
            }
            sum = maxSubArraySum(temp, y);
            if (sum > maxSum)
                maxSum = sum;
        }
    }
    ofstream out("output.txt");
    out << maxSum;
}

int maxSubArraySum(int *array, int size) {
    int tmp = 0;
    int tmpMax = 0;

    for (int i = 0; i < size; ++i) {
        tmp += array[i];
        if (tmp < 0) {
            tmp = 0;
        }
        tmpMax = max(tmpMax, tmp);
    }
    return tmpMax;
}


void maxSubArrayum() {
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
}
