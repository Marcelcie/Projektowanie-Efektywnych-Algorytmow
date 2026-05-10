//Projekt 3 - Algorytm Wyżarzania, Marcel Cieśliński 280871
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "SimulatedAnnealing.h"

using namespace std;

vector<vector<int>> LoadMatrixFromFile(string filename) {
    ifstream plik(filename);
    vector<vector<int>> matrix;

    if (!plik.is_open()) {
        cout << "Błąd: Nie można otworzyć pliku " << filename << "!" << endl;
        return matrix;
    }

    string line;
    int N = 0;
    bool isTSPLIB = false;

    // 1. Analiza nagłówka (szukamy rozmiaru N i sprawdzamy czy to format TSPLIB)
    while (getline(plik, line)) {
        if (line.find("DIMENSION") != string::npos) {
            stringstream ss(line);
            string temp;
            while (ss >> temp) {
                if (stringstream(temp) >> N) break;
            }
        }
        else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
            isTSPLIB = true;
            break; // Tutaj kończą się napisy, a zaczynają liczby
        }
    }

    // Jeśli to nie był plik TSPLIB (tylko np. stary .txt), wracamy na start
    if (!isTSPLIB) {
        plik.clear();
        plik.seekg(0, ios::beg);
        plik >> N;
    }

    // 2. Przygotowanie wektora o rozmiarze N x N
    if (N > 0) {
        matrix.resize(N, vector<int>(N));

        // 3. Wczytywanie danych (liczb) do macierzy
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!(plik >> matrix[i][j])) break;

                // Zamieniamy 9999 oraz przekątną na naszą "nieskończoność" (-1)
                if (i == j || matrix[i][j] >= 9999) {
                    matrix[i][j] = -1;
                }
            }
        }
        cout << "Sukces! Wczytano macierz o rozmiarze: " << N << "x" << N << endl;
    }

    plik.close();
    return matrix;
}

int main() {
    vector<vector<int>> matrix;
    double timeLimit = 60.0;
    double t_start = 10000.0;
    double alpha = 0.99;
    int epochMultiplier = 10;
    CoolingSchedule sched = GEOMETRIC;
    InitialSolutionType initSol = RANDOM_PATH;
    int choice;

    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1. Wczytanie danych z pliku" << endl;
        cout << "2. Wprowadzenie kryterium stopu (czas) [" << timeLimit << "s]" << endl;
        cout << "3. Obliczanie rozwiazania poczatkowego [1-Losowe, 2-Zachlanne]" << endl;
        cout << "4. Modyfikacje algorytmu (temperatura, chlodzenie)" << endl;
        cout << "5. Uruchomienie algorytmu" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string fn; cout << "Nazwa pliku: "; cin >> fn;
            matrix = LoadMatrixFromFile(fn);
            break;
        }
        case 2:
            cout << "Czas (s): "; cin >> timeLimit; break;
        case 3: {
            int s; cout << "1-Losowe, 2-Zachlanne: "; cin >> s;
            initSol = (s == 2) ? GREEDY_PATH : RANDOM_PATH; break;
        }
        case 4: {
            cout << "Temperatura startowa: "; cin >> t_start;
            int sc; cout << "Schemat (1-Geom, 2-Lin): "; cin >> sc;
            sched = (sc == 2) ? LINEAR : GEOMETRIC;
            cout << "Wspolczynnik chlodzenia: "; cin >> alpha;
            cout << "Mnoznik epoki: "; cin >> epochMultiplier;
            break;
        }
        case 5: {
            if (matrix.empty()) { cout << "Brak macierzy!" << endl; break; }
            SimulatedAnnealing sa(matrix);
            sa.SetTimeLimit(timeLimit);
            sa.SetParameters(t_start, alpha, matrix.size() * epochMultiplier, sched, initSol);

            cout << "\nUruchamiam algorytm..." << endl;

            // START STOPERA
            auto start_time = chrono::high_resolution_clock::now();

            sa.Run();

            // KONIEC STOPERA
            auto end_time = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end_time - start_time;

            cout << "\n--- WYNIKI ---" << endl;
            cout << "Czas wykonania: " << elapsed.count() << " s" << endl;
            cout << "Najlepszy Koszt: " << sa.GetBestCost() << endl;
            cout << "Trasa: ";
            for (int c : sa.GetBestPath()) cout << c << " ";
            cout << sa.GetBestPath()[0] << endl;
            break;
        }
        }
    } while (choice != 0);

    return 0;
}