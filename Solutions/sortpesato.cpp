#include <fstream>
#include <vector>
#include <climits>

using namespace std;
vector<int> myVector;

//Contiene quali posizioni sono state processate
vector<bool> visited;

int main(void) {
    ifstream in("input.txt");
    int N;
    in >> N;
    myVector.resize(N + 1);
    visited.resize(N + 1, false);
    for (int i = 1; i <= N; i++)
        in >> myVector[i];
    int moves = 0;
    int price = 0;
    for (int i = 1; i <= N; i++) {
        //Se non abbiamo ancora lavorato con questa posizione
        if (!visited[i]) {
            if (myVector[i] != i) {
                //Mantengo la somma del ciclo di posizioni
                int sum = 0;
                //Il piú piccolo degli interi di questo ciclo
                int mn = INT_MAX;
                int current = i;
                int num = 0;
                while (!visited[myVector[current]]) {
                    mn = min(mn, myVector[current]);
                    sum += myVector[current];
                    visited[myVector[current]] = true;
                    current = myVector[current];
                    num++;
                }
                //Prezzo senza "prendere in prestito"
                int currentPrice = (num - 1) * mn + sum - mn;
                //Prezzo prendendo in prestito
                if (mn != 1)
                    currentPrice = min(currentPrice, 2 * (1 + mn) + num - 1 + sum - mn);
                price += currentPrice;
                //Un ciclo di num interi ha bisogno di num-1 moves
                moves += num - 1;
            }
            visited[i] = true;
        }
    }
    ofstream out("output.txt");
    out << moves << " " << price << endl;
    return 0;
}

