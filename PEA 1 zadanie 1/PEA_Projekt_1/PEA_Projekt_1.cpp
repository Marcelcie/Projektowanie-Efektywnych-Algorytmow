#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void matrix_reader(string file) {
    
    ifstream plik(file);
        if (plik.is_open()) {
            int N;
            plik >> N;
            int* matrix = new int[N * N];
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N ; j++) {
                    plik >> matrix[i * N + j];
                    cout << matrix[i * N + j] << "\t";
                }
                cout << endl;
            }
            plik.close();      // Zamykamy plik
            delete[] matrix;   // Zwalniamy pamięć, aby uniknąć wycieków!
        }
        else {
            cout << "brak wybranego pliku wykonawczego!";
        }

}

int main()
{
    matrix_reader("C:\\Users\\ciesl\\source\\repos\\Projektowanie-Efektywnych-Algorytmow\\PEA 1 zadanie 1\\PEA_Projekt_1\\x64\\Release\\tsp_6_2.txt");
}
