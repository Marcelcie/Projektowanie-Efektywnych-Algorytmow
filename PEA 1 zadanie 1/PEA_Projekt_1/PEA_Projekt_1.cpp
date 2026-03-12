#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>


using namespace std;
using namespace std::chrono;

class ATSP {
    //klasa komiwojażer
private:
    int* matrix;
    int N;
public:
    //konstruktor
    ATSP() {
        matrix = nullptr;
        N = 0;
    }
    //Zwalnianie pamięci ram unikniecie memory leaku.
    ~ATSP() {
        if (matrix != nullptr) {
            delete[] matrix;
        }
    }

    void matrix_reader(string file) {
        //Funkcja, która wczytuje plik .txt z danymi i sprawdza czy podana ścieżka istnieje.
        ifstream plik(file);
        if (plik.is_open()) {
            plik >> N;
            if (matrix != nullptr) {
                delete[] matrix;
            }
            matrix = new int[N * N];
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    plik >> matrix[i * N + j];
                    //cout << matrix[i * N + j] << "\t";  -> Test poprawności wyświetlania danych z pliku.
                }
                cout << endl;
            }
            plik.close();   // Zamykamy plik
        }
        else {
            cout << "brak wybranego pliku wykonawczego!";
        }

    }
    void Print_Matrix() {
        if (matrix == nullptr) return;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << matrix[i * N + j] << "\t";
            }
            cout << endl;
        }
    }
    void Nearest_Neighbor() {
        if (matrix == nullptr) {
            cout << "Blad! Macierz jest pusta. Wczytaj najpierw dane z pliku.\n";
            return;
        }
        bool* odwiedzone = new bool[N];
        for (int i = 0; i < N; i++) {
            odwiedzone[i] = false;
        }
        int aktualne_miasto = 0;
        odwiedzone[aktualne_miasto] = true;
        int calkowity_koszt = 0;

        cout << "\n--- TRASA (NN) ---\n";
        cout << aktualne_miasto;


        for (int krok = 1; krok < N; krok++) {
            int najblizsze_miasto = -1;
            int min_koszt = 9999999;

            for (int sasiad = 0; sasiad < N; sasiad++) {
                int koszt_przejscia = matrix[aktualne_miasto * N + sasiad];

                if (!odwiedzone[sasiad] && koszt_przejscia != -1 && koszt_przejscia < min_koszt) {
                    min_koszt = koszt_przejscia;
                    najblizsze_miasto = sasiad;
                }
            }
            aktualne_miasto = najblizsze_miasto;
            odwiedzone[aktualne_miasto] = true;
            calkowity_koszt += min_koszt;

            cout << " -> " << aktualne_miasto;
        }
        int koszt_powrotu = matrix[aktualne_miasto * N + 0];
        calkowity_koszt += koszt_powrotu;

        cout << " -> 0\n";
        cout << "Calkowity koszt: " << calkowity_koszt << "\n";


        delete[] odwiedzone;

    }
    void Random_Search(int iteracje) {
        if (matrix == nullptr) {
            cout << "Blad! Macierz jest pusta. Wczytaj najpierw dane z pliku.\n";
            return;
        }

        int* obecna_trasa = new int[N];
        int* najlepsza_trasa = new int[N];

        for (int i = 0; i < N; i++) {
            obecna_trasa[i] = i;
            najlepsza_trasa[i] = i;
        }

        int min_koszt = 9999999;
        srand(time(NULL));

        for (int k = 0; k < iteracje; k++) {

            // POPRAWIONE TASOWANIE (bez dzielenia przez zero)
            for (int i = N - 1; i > 1; i--) {
                int j = 1 + rand() % i;
                int temp = obecna_trasa[i];
                obecna_trasa[i] = obecna_trasa[j];
                obecna_trasa[j] = temp;
            }

            // POPRAWIONE LICZENIE KOSZTU (bez wychodzenia za tablice)
            int aktualny_koszt = 0;
            for (int i = 0; i < N - 1; i++) {
                aktualny_koszt += matrix[obecna_trasa[i] * N + obecna_trasa[i + 1]];
            }
            aktualny_koszt += matrix[obecna_trasa[N - 1] * N + obecna_trasa[0]];

            if (aktualny_koszt < min_koszt) {
                min_koszt = aktualny_koszt;
                for (int i = 0; i < N; i++) {
                    najlepsza_trasa[i] = obecna_trasa[i];
                }
            }
        }

        cout << "\n--- TRASA (Algorytm Losowy - " << iteracje << " prob) ---\n";
        for (int i = 0; i < N; i++) {
            cout << najlepsza_trasa[i] << " -> ";
        }
        cout << najlepsza_trasa[0] << "\n";
        cout << "Calkowity koszt: " << min_koszt << "\n";

        delete[] obecna_trasa;
        delete[] najlepsza_trasa;
    }
};



int main()
{
    //Inicjalizacja Obiektu w programie
    ATSP problem;
    //deklaracja zmiennych
    int wybor; string sciezka;

    do {
        //Interaktywne menu
        cout << "\n=== MENU GLOWNE ===" << endl;
        cout << "1. Wczytaj macierz z pliku" << endl;
        cout << "2. Wyswietl macierz" << endl;
        cout << "3. Wyswietlanie marcierzy NN" << endl;
		cout << "4. Wyswietlanie marcierzy Losowy" << endl;
        // ... dopisz reszte opcji (NN, Brute-Force, itp.)
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;
        switch (wybor) {
        case 1:
            cout << "Podaj sciezke do swojego pliku .txt: \n";
            cin.ignore();          // Czyścimy bufor z Entera po 'cin >> wybor'
            getline(cin, sciezka); // Pobieramy pełną ścieżkę ze spacjami
            problem.matrix_reader(sciezka);
            cout << "Wczytano sciezke do pamieci programu." << endl;
            break;
        case 2:
            cout << endl;
            //C:\\Users\\ciesl\\source\\repos\\Projektowanie-Efektywnych-Algorytmow\\PEA 1 zadanie 1\\PEA_Projekt_1\\x64\\Release\\tsp_6_2.txt -> przykładowy plik do wczytywania.
            problem.Print_Matrix();
            break;
        case 3: {
            cout << endl;
            auto start = high_resolution_clock::now();
            problem.Nearest_Neighbor();
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;
            cout << "Czas wykonania algorytmu NN: " << czas.count() << " ms\n";
            break;
            }
        case 4: {
            int liczba_prob = 0;
            cout << "Podaj ilosc permutacji, ktore chcesz wykonac: ";
            cin >> liczba_prob;

            auto start = high_resolution_clock::now();
            problem.Random_Search(liczba_prob);
            auto end = high_resolution_clock::now();

            duration<double, milli> czas = end - start;
            cout << "Czas wykonania algorytmu Losowego: " << czas.count() << " ms\n";
            break;
        }
        default:
            cout << "Blad nieprawidlowa liczba!";
        }
    } while (wybor != 0);
    return 0;
}
