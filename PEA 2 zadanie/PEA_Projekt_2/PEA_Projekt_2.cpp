//projekt 2 
// autor: Marcel Cieśliński nr.albumu 280871

#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include "algorytmy.h"

using namespace std;
using namespace std::chrono;

class ATSP {
private:
    vector<vector<int>> Matrix;
    int N;
public:
    //konstruktor
    ATSP() {
        N = 0;
    }
	//destruktor
    ~ATSP() {}

    vector<vector<int>>getMatrix() {
        return Matrix;
    }



void readMatrixFromFile(string file) {
    ifstream plik(file);
    if (plik.is_open()) {
        plik >> N;

        //Przygotowanie macierzy NxN
        Matrix.assign(N, vector<int>(N));

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                plik >> Matrix[i][j];
            }
        }
        plik.close();
		cout << "Macierz o rozmiarze:" << N << "x" << N << "została wczytana pomyślnie." << endl;
    }
    else {
		cout << "Nie można otworzyć pliku: " << file << endl;
    }
}
void DisplayMatrix() {
    if (Matrix.empty()) {
        cout << "Macierz jest pusta. Wczytaj dane z pliku." << endl;
        return;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << Matrix[i][j] << "\t";
        }
        cout << endl;
    }
}
void Generate_Matrix(int rozmiar_macierzy) {
	N = rozmiar_macierzy;
	Matrix.assign(N, vector<int>(N));
	srand(time(NULL)); // Inicjalizacja generatora liczb losowych

    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j) {
            if (i == j) {
				Matrix[i][j] = -1; // Brak przejścia do samego siebie
            }
            else {
				Matrix[i][j] = rand() % 100 + 1; // Losowa waga krawędzi (1-100)    
            }
        }
    }
	cout << "Wygenerowano macierz o rozmiarze:" << N << "x" << N << "." << endl;
}
};

int main()
{
    //inicjacja obiektu w projekcie
    ATSP problem;
    string scieszka;
    int wybor, n_rozmiar;

    do {
        cout << "\n=== MENU GLOWNE (Projekt 2 - Branch & Bound) ===" << endl;
        cout << "1. Wczytaj macierz z pliku" << endl;
        cout << "2. Wyswietl macierz" << endl;
        cout << "3. Generowanie Losowych macierzy" << endl;
        cout << "4. Uruchom B&B (Best-First / Kopiec) - na 3.0" << endl;
        cout << "5. Uruchom B&B (Breadth-First / Kolejka) - na 4.0" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor) {
        case 1:
            cout << "Podaj scieszke do pliku: ";
            cin >> scieszka;
			problem.readMatrixFromFile(scieszka);
            break;
        case 2:
            problem.DisplayMatrix();
			break;
        case 3:
            cout << "Podaj rozmiar macierzy: ";
            cin >> n_rozmiar;
			problem.Generate_Matrix(n_rozmiar);
            break;
        case 4: {
            vector<vector<int>> macierz = problem.getMatrix();
            if (macierz.empty()) {
                cout << "Macierz jest pusta. Wczytaj dane z pliku " << endl;
                break;
            }
            auto stary = high_resolution_clock::now();
            int koszt = SolveTSP(macierz);
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - stary;
            cout << "Najlepszy koszt trasy: " << koszt << endl;
            cout << "Czas wykonania: " << czas.count() << " ms" << endl;
            break;
        }
        case 5: {
            vector<vector<int>> macierz = problem.getMatrix();
            if (macierz.empty()) {
                cout << "Blad! Macierz jest pusta. Wczytaj dane z pliku lub wygeneruj.\n";
                break;
            }

            cout << "\n[ Trwaja obliczenia - Breadth-First Search... ]\n";
            auto start = high_resolution_clock::now();

            // CZYSTE LICZENIE BEZ COUT'OW
            int koszt = SolveTSPBreadth(macierz);

            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;

            cout << "Znalazlem najtansza trase! Koszt: " << koszt << "\n";
            cout << "Czas dzialania algorytmu: " << czas.count() << " ms\n";
            break;
        }
        case 0:
            cout << "Zamykanie programu..." << endl;
            break;

        default:
            cout << "Blad! Nieprawidlowa opcja." << endl;
        }

	} while (wybor != 0);

    return 0;
}

