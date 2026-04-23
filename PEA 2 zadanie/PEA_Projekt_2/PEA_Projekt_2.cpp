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
        srand(time(NULL)); // Inicjalizacja generatora liczb losowych
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
		cout << "Macierz o rozmiarze: " << N << "x" << N << " zostala wczytana pomyslnie." << endl;
    }
    else {
		cout << "Nie można otworzyc pliku: " << file << endl;
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
void Automat_BB(int poczatkowe_N, int koncowe_N, int ilosc_prob) {
    ofstream plik("wyniki_bb.csv", ios::app);
    if (!plik.is_open()) {
        cout << "Blad otwarcia pliku CSV!" << endl;
        return;
    }

    // Dodajemy nagłówek, jeśli plik jest pusty
    plik.seekp(0, ios::end);
    if (plik.tellp() == 0) {
        plik << "N;Metoda;Sredni_Czas_ms\n";
    }

    cout << "\n--- ROZPOCZECIE TESTOW AUTOMATYCZNYCH ---\n";

    for (int n = poczatkowe_N; n <= koncowe_N; n++) {
        double suma_czasow_kopiec = 0.0;
        double suma_czasow_kolejka = 0.0;

        for (int p = 0; p < ilosc_prob; p++) {
            Generate_Matrix(n);

            // Atrapa ścieżki dla automatu (bo mierzymy tylko czas)
            vector<int> dummy_path;

            // --- NOWE: LICZYMY BUDZET STARTOWY ---
            int bezpieczny_budzet = ObliczPoczatkowyKoszt(Matrix);

            // --- TEST BEST-FIRST (Kopiec) ---
            auto start_kopiec = high_resolution_clock::now();
            SolveTSP(Matrix,dummy_path, bezpieczny_budzet); // <-- Przekazujemy budżet!
            auto end_kopiec = high_resolution_clock::now();
            suma_czasow_kopiec += duration<double, milli>(end_kopiec - start_kopiec).count();

            // --- TEST BREADTH-FIRST (Kolejka FIFO) ---
            auto start_kolejka = high_resolution_clock::now();
            SolveTSPBreadth(Matrix,dummy_path, bezpieczny_budzet); // <-- Przekazujemy budżet!
            auto end_kolejka = high_resolution_clock::now();
            suma_czasow_kolejka += duration<double, milli>(end_kolejka - start_kolejka).count();
        }

        // Uśredniamy wyniki
        double sr_czas_kopiec = suma_czasow_kopiec / ilosc_prob;
        double sr_czas_kolejka = suma_czasow_kolejka / ilosc_prob;

        // Zapis do pliku CSV
        plik << n << ";Best-First(Kopiec);" << sr_czas_kopiec << "\n";
        plik << n << ";Breadth-First(Kolejka);" << sr_czas_kolejka << "\n";

        cout << "Przetestowano N=" << n << " | Kopiec: " << sr_czas_kopiec << " ms | Kolejka: " << sr_czas_kolejka << " ms\n";
    }

    plik.close();
    cout << "--- ZAKONCZONO! Wyniki zapisano w pliku wyniki_bb.csv ---\n";
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
		cout << "6. Uruchom Automatyczne Testy B&B (Best-First vs Breadth-First)" << endl;
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
            vector<int> najlepsza_trasa;
            int bezpieczny_budzet = ObliczPoczatkowyKoszt(macierz);
            auto stary = high_resolution_clock::now();
            int koszt = SolveTSP(macierz, najlepsza_trasa, bezpieczny_budzet);
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - stary;
            cout << "Najlepszy koszt trasy: " << koszt << endl;
            cout << "Ciag wierzcholkow: ";
            if (!najlepsza_trasa.empty()) {
                for (size_t i = 0; i < najlepsza_trasa.size(); ++i) {
                    cout << najlepsza_trasa[i] << (i == najlepsza_trasa.size() - 1 ? "" : " -> ");
                }
            }
            else {
                cout << "Trasa optymalna znaleziona przez algorytm zachlanny (budzet startowy).";
            }
            cout << endl;
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
            vector<int> najlepsza_trasa;
            int bezpieczny_budzet = ObliczPoczatkowyKoszt(macierz);
            auto start = high_resolution_clock::now();
            // CZYSTE LICZENIE BEZ COUT'OW
            int koszt = SolveTSPBreadth(macierz,najlepsza_trasa, bezpieczny_budzet);

            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;

            cout << "Znalazlem najtansza trase! Koszt: " << koszt << "\n";
            cout << "Ciag wierzcholkow: ";
            if (!najlepsza_trasa.empty()) {
                // Jeśli B&B znalazło trasę lub potwierdziło trasę NN
                for (size_t i = 0; i < najlepsza_trasa.size(); ++i) {
                    cout << najlepsza_trasa[i] << (i == najlepsza_trasa.size() - 1 ? "" : " -> ");
                }
            }
            else {
                cout << "Trasa optymalna znaleziona przez algorytm zachlanny (budzet startowy).";
            }
            cout << endl;
            cout << "Czas dzialania algorytmu: " << czas.count() << " ms\n";
            break;
        }
        case 6:{
                  int start_n, end_n, proby;
                  cout << "Podaj poczatkowy rozmiar N: ";
                  cin >> start_n;
                  cout << "Podaj koncowy rozmiar N: ";
                  cin >> end_n;
                  cout << "Podaj ilosc prob dla kazdego N: ";
                  cin >> proby;
                  problem.Automat_BB(start_n, end_n, proby);
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

