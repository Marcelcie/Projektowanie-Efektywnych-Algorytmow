#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void matrix_reader(string file) {
    //Funkcja, która wczytuje plik .txt z danymi i sprawdza czy podana ścieżka istnieje.
    ifstream plik(file);
        if (plik.is_open()) {
            int N;
            plik >> N;
            int* matrix = new int[N * N];
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N ; j++) {
                    plik >> matrix[i * N + j];
                    //cout << matrix[i * N + j] << "\t";  -> Test poprawności wyświetlania danych z pliku.
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
void Print_Matrix(int* matrix,int N) {
    
    if (matrix != nullptr) {
        delete[] matrix;
    }

    int* matrix = new int[N * N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i * N + j] << "\t";
        }
        cout << endl;
    }
}

int main()
{
    //deklaracja zmiennych
    int wybor; string sciezka; int* matrix = nullptr; int N = 0;
    //koniec deklaracji

    do {
        //Interaktywne menu
        cout << "\n=== MENU GLOWNE ===" << endl;
        cout << "1. Wczytaj macierz z pliku" << endl;
        cout << "2. Wyswietl macierz" << endl;
        // ... dopisz reszte opcji (NN, Brute-Force, itp.)
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;
        switch (wybor) {
        case 1:
            cout << "Podaj sciezke do swojego pliku .txt: \n";
            cin.ignore();          // Czyścimy bufor z Entera po 'cin >> wybor'
            getline(cin, sciezka); // Pobieramy pełną ścieżkę ze spacjami
            cout << "Wczytano sciezke do pamieci programu." << endl;
            break;
        case 2:
            cout << endl;
            //C:\\Users\\ciesl\\source\\repos\\Projektowanie-Efektywnych-Algorytmow\\PEA 1 zadanie 1\\PEA_Projekt_1\\x64\\Release\\tsp_6_2.txt -> przykładowy plik do wczytywania.
            Print_Matrix(matrix,N);
            break;
        default:
            cout << "Blad nieprawidlowa liczba!";
        }
    } while(wybor != 0);

    //zwalnianie pamięci przed zakończeniem programu.
    if (matrix != nullptr) {
        delete[] matrix;
    }
}
