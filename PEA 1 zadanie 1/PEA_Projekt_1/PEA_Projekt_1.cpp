
// Marcel Cieslinski, 280871
// Projekt nr 1 - ATSP (BF, NN, RNN, RS)

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
    //Zwalnianie pamięci ram, unikniecie memory leaku.
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
    void Automat(int Rozmiar_macierzy, int Ilosc_testow) {
        double suma_czasow_NN = 0.0, suma_bledu_NN = 0.0;
        double suma_czasow_RS = 0.0, suma_bledu_RS = 0.0;
		double suma_czasow_RNN = 0.0, suma_bledu_RNN = 0.0;
		double suma_czasow_BF = 0.0;
        int liczba_prob_RS = 1000;

        for (int i = 0; i < Ilosc_testow; i++) {
            Generate_Matrix(Rozmiar_macierzy);
            int koszt_idealny = Brute_Force(false);

            // --- TEST NN ---
            auto start_NN = high_resolution_clock::now();
            int koszt_NN = Nearest_Neighbor(false);
            auto end_NN = high_resolution_clock::now();

            duration<double, milli> czas_NN = end_NN - start_NN;
            suma_czasow_NN += czas_NN.count();
            suma_bledu_NN += ((double)koszt_NN - koszt_idealny) / koszt_idealny * 100.0;


			// --- TEST BF ---
			auto start_BF = high_resolution_clock::now();
			int koszt_BF = Brute_Force(false);
			auto end_BF = high_resolution_clock::now();

			duration<double, milli> czas_BF = end_BF - start_BF;
			suma_czasow_BF += czas_BF.count();


            // --- TEST RANDOM SEARCH ---
            auto start_RS = high_resolution_clock::now();
            int koszt_RS = Random_Search(liczba_prob_RS, false);
            auto end_RS = high_resolution_clock::now();

            duration<double, milli> czas_RS = end_RS - start_RS;
            suma_czasow_RS += czas_RS.count();
            suma_bledu_RS += ((double)koszt_RS - koszt_idealny) / koszt_idealny * 100.0;

            // --- TEST RNN ---
            auto start_RNN = high_resolution_clock::now();
            int koszt_RNN = Repetitive_Nearest_Neighbor(false);
            auto end_RNN = high_resolution_clock::now();

            duration<double, milli> czas_RNN = end_RNN - start_RNN;
            suma_czasow_RNN += czas_RNN.count();
            suma_bledu_RNN += ((double)koszt_RNN - koszt_idealny) / koszt_idealny * 100.0;

        }

        // Liczenie średnich
        double sr_c_NN = suma_czasow_NN / Ilosc_testow;
        double sr_b_NN = suma_bledu_NN / Ilosc_testow;
        double sr_c_RS = suma_czasow_RS / Ilosc_testow;
        double sr_b_RS = suma_bledu_RS / Ilosc_testow;
		double sr_c_RNN = suma_czasow_RNN / Ilosc_testow;
		double sr_b_RNN = suma_bledu_RNN / Ilosc_testow;
		double sr_c_BF = suma_czasow_BF / Ilosc_testow;

        // Wyświetlanie w konsoli
        cout << "\n--- WYNIKI (N=" << Rozmiar_macierzy << ", Proby=" << Ilosc_testow << ") ---" << endl;
		cout << "BF | Czas: " << sr_c_BF << "ms | Blad: 0 %" << endl;
        cout << "NN | Czas: " << sr_c_NN << "ms | Blad: " << sr_b_NN << "%" << endl;
        cout << "RS | Czas: " << sr_c_RS << "ms | Blad: " << sr_b_RS << "%" << endl;
		cout << "RNN | Czas: " << sr_c_RNN << "ms | Blad: " << sr_b_RNN << "%" << endl;

        // ZAPIS DO PLIKU CSV
        ofstream plik("wyniki_atsp.csv", ios::app);
        if (plik.is_open()) {
            // Nagłówek (tylko jeśli plik jest pusty)
            plik.seekp(0, ios::end);
            if (plik.tellp() == 0) {
                plik << "N;Algorytm;Sredni_Czas_ms;Sredni_Blad_procent\n";
            }
            // Dane
			plik << Rozmiar_macierzy << ";BF;" << sr_c_BF << ";0\n";
            plik << Rozmiar_macierzy << ";NN;" << sr_c_NN << ";" << sr_b_NN << "\n";
            plik << Rozmiar_macierzy << ";RS;" << sr_c_RS << ";" << sr_b_RS << "\n";
			plik << Rozmiar_macierzy << ";RNN;" << sr_c_RNN << ";" << sr_b_RNN << "\n";
            plik.close();
            cout << "Wyniki zapisano do wyniki_atsp.csv" << endl;
        }
    }
    void Generate_Matrix(int Rozmiar_macierzy) {
        if (matrix != nullptr) {
			delete[] matrix;
        }
        N = Rozmiar_macierzy;
        matrix = new int[N * N];
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j) {
                    matrix[i * N + j] = -1; // Brak przejścia do samego siebie
                }
                else {
                    matrix[i * N + j] = rand() % 100 + 1; // Losowy koszt między 1 a 100
                }
            }
        }
	}
    int Nearest_Neighbor(bool pokaz_trase = true,int startowe=0) {
        if (matrix == nullptr) {
            if (pokaz_trase) cout << "Blad! Macierz jest pusta.\n";
            return -1;
        }

        bool* odwiedzone = new bool[N];
        for (int i = 0; i < N; i++) odwiedzone[i] = false;

        int aktualne_miasto = startowe;
        odwiedzone[aktualne_miasto] = true;
        int calkowity_koszt = 0;

        if (pokaz_trase) {
            cout << "\n--- TRASA (NN) ---\n" << aktualne_miasto;
        }

        for (int krok = 1; krok < N; krok++) {
            int najblizsze_miasto = -1;
            int min_koszt = INT_MAX;

            // SZUKANIE NAJBLIZSZEGO SASIADA
            for (int sasiad = 0; sasiad < N; sasiad++) {
                int koszt_przejscia = matrix[aktualne_miasto * N + sasiad];
                if (!odwiedzone[sasiad] && koszt_przejscia != -1 && koszt_przejscia < min_koszt) {
                    min_koszt = koszt_przejscia;
                    najblizsze_miasto = sasiad;
                }
            }

            // WYKONANIE SKOKU
            if (najblizsze_miasto != -1) {
                aktualne_miasto = najblizsze_miasto;
                odwiedzone[aktualne_miasto] = true;
                calkowity_koszt += min_koszt;
                if (pokaz_trase) cout << " -> " << aktualne_miasto;
            }
            else {
                if (pokaz_trase) cout << " -> Brak Polaczenia!";
                delete[] odwiedzone;
                return -1;
            }
        }

        // POWRÓT
        int koszt_powrotu = matrix[aktualne_miasto * N + startowe];
        calkowity_koszt += koszt_powrotu;
        if (pokaz_trase) cout << " -> 0\nCalkowity koszt: " << calkowity_koszt << "\n";

        delete[] odwiedzone;
        return calkowity_koszt;
    }
    int Random_Search(int iteracje, bool pokaz_trase = true) {
        if (matrix == nullptr) {
            if (pokaz_trase) cout << "Blad! Macierz jest pusta. Wczytaj najpierw dane z pliku.\n";
            return -1;
        }

        int* obecna_trasa = new int[N];
        int* najlepsza_trasa = new int[N];

        for (int i = 0; i < N; i++) {
            obecna_trasa[i] = i;
            najlepsza_trasa[i] = i;
        }

        int min_koszt = INT_MAX;
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
        return min_koszt;
    }
    int Brute_Force(bool pokaz_trase = true) {
        if (matrix == nullptr) {
            cout << "Blad! Macierz jest pusta. Wczytaj najpierw dane z pliku.\n";
            return -1;
        }
        int* trasa = new int[N];
        for (int i = 0; i < N; i++) {
            trasa[i] = i;
        }
        int min_koszt = INT_MAX;
        do {
            int aktualny_koszt = 0;
            for (int i = 0; i < N - 1; i++) {
                aktualny_koszt += matrix[trasa[i] * N + trasa[i + 1]];
            }
            aktualny_koszt += matrix[trasa[N - 1] * N + trasa[0]];
            if (aktualny_koszt < min_koszt) {
                min_koszt = aktualny_koszt;
                if (pokaz_trase) {
                    cout << "\n--- NOWA NAJLEPSZA TRASA (BF) ---\n";
                    for (int j = 0; j < N; j++) {
                        cout << trasa[j] << " -> ";
                    }
                    cout << trasa[0] << "\n";
                    cout << "Calkowity koszt: " << min_koszt << "\n";
                }
            }
        } while (next_permutation(trasa + 1, trasa + N));
        delete[] trasa;
		return min_koszt;
    }
    int Repetitive_Nearest_Neighbor(bool pokaz_trase = true) {
        if (matrix == nullptr) return -1;

        int najlepszy_koszt = INT_MAX;

        for (int i = 0; i < N; i++) {
            // Wywołujemy NN dla każdego miasta startowego bez śmiecenia w konsoli
            int koszt = Nearest_Neighbor(false, i);

            if (koszt < najlepszy_koszt && koszt != -1) {
                najlepszy_koszt = koszt;
            }
        }

        if (pokaz_trase) {
            cout << "\n--- WYNIK RNN (Najlepszy z " << N << " startow) ---\n";
            cout << "Najlepszy koszt: " << najlepszy_koszt << endl;
        }
        return najlepszy_koszt;
    }
};



int main()
{
    //Inicjalizacja Obiektu w programie
    ATSP problem;
    //deklaracja zmiennych
    string sciezka; int wybor, n_rozmiar, n_powtorzen, liczba_prob;

    do {
        //Interaktywne menu
        cout << "\n=== MENU GLOWNE ===" << endl;
        cout << "1. Wczytaj macierz z pliku" << endl;
        cout << "2. Wyswietl macierz" << endl;
        cout << "3. Generowanie Losowych macierzy" << endl;
        cout << "4. Automat testowy (Porownanie: BF, NN, RS, RNN)" << endl;
        cout << "5. Wyswietlanie trasy NN" << endl;
        cout << "6. Wyswietlanie trasy Losowy (RS)" << endl;
        cout << "7. Wyswietlanie trasy Brute-Force" << endl;
        cout << "8. Wyswietlanie trasy RNN" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor) {
        case 1: // wczytywanie macierzy z pliku
            cout << "Podaj sciezke do pliku .txt: ";
            cin >> sciezka;
            problem.matrix_reader(sciezka);
            cout << "Pomyslnie zaladowano plik: " << sciezka << endl;
            break;

        case 2: //Wyswietlanie macierzy
            problem.Print_Matrix();
            break;

        case 3: //Generowanie losowych macierzy
            cout << "Podaj rozmiar macierzy: ";
            cin >> n_rozmiar;
            problem.Generate_Matrix(n_rozmiar);
            cout << "Wygenerowano macierz " << n_rozmiar << "x" << n_rozmiar << endl;
            break;

        case 4: { //Automat testowy
            cout << "--- AUTOMAT TESTOWY ---" << endl;
            cout << "Rozmiar (N): "; cin >> n_rozmiar;
            cout << "Powtorzenia: "; cin >> n_powtorzen;
            problem.Automat(n_rozmiar, n_powtorzen);
            break;
        }

        case 5: { //NN
            auto start = high_resolution_clock::now();
            int koszt = problem.Nearest_Neighbor(true);
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;
            if (koszt != -1) cout << "Czas NN: " << czas.count() << " ms" << endl;
            break;
        }

        case 6: { //Losowy
            cout << "Podaj ilosc prob: ";
            cin >> liczba_prob;
            auto start = high_resolution_clock::now();
            int koszt = problem.Random_Search(liczba_prob, true);
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;
            if (koszt != -1) cout << "Czas RS: " << czas.count() << " ms" << endl;
            break;
        }

        case 7: { //Brute-Force
            auto start = high_resolution_clock::now();
            int koszt = problem.Brute_Force(true);
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;
            if (koszt != -1) cout << "Czas BF: " << czas.count() << " ms" << endl;
            break;
        }

        case 8: {//RNN
            auto start = high_resolution_clock::now();
            int koszt = problem.Repetitive_Nearest_Neighbor(true);
            auto end = high_resolution_clock::now();
            duration<double, milli> czas = end - start;
            if (koszt != -1) cout << "Czas RNN: " << czas.count() << " ms" << endl;
            break;
        }

        case 0:
            cout << "Zamykanie programu..." << endl;
            break;

        default:
            cout << "Blad nieprawidlowa opcja!" << endl;
        }
    } while (wybor != 0);
    return 0;
}
