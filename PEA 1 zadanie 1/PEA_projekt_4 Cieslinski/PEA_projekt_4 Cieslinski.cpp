#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <random>
#include <iomanip>

using namespace std;

// Klasa reprezentujaca pojedyncza trase (chromosom)
class Osobnik {
public:
    vector<int> trasa;
    int fitness; // Koszt trasy

    Osobnik() : fitness(0) {}
};

// Klasa glowna zarzadzajaca algorytmem genetycznym
class AlgorytmGenetyczny {
private:
    vector<vector<int>> macierzKosztow;
    int liczbaMiast;

    // Parametry algorytmu sterowane z menu
    int rozmiarPopulacji;
    double wspolczynnikMutacji;
    double wspolczynnikKrzyzowania;
    int metodaMutacji; // 1 - Swap, 2 - Inwersja
    double limitCzasu; // w sekundach

    // Generator liczb losowych
    mt19937 rng;

    // Pomocnicza funkcja do liczenia kosztu danej trasy
    int obliczKosztTrasy(const vector<int>& trasa) {
        int koszt = 0;
        for (int i = 0; i < liczbaMiast - 1; ++i) {
            koszt += macierzKosztow[trasa[i]][trasa[i + 1]];
        }
        koszt += macierzKosztow[trasa[liczbaMiast - 1]][trasa[0]]; // powrot do miasta startowego
        return koszt;
    }

public:
    AlgorytmGenetyczny() {
        liczbaMiast = 0;
        rozmiarPopulacji = 100;        // domyslny rozmiar populacji
        wspolczynnikMutacji = 0.05;    // domyslna szansa mutacji (5%)
        wspolczynnikKrzyzowania = 0.8; // domyslna szansa krzyzowania (80%)
        metodaMutacji = 1;             // domyslnie Swap
        limitCzasu = 10.0;             // domyslnie 10 sekund
        rng.seed(chrono::system_clock::now().time_since_epoch().count());
    }

    void ustawRozmiarPopulacji(int rozmiar) { rozmiarPopulacji = rozmiar; }
    void ustawWspolczynnikMutacji(double pm) { wspolczynnikMutacji = pm; }
    void ustawWspolczynnikKrzyzowania(double pc) { wspolczynnikKrzyzowania = pc; }
    void ustawMetodeMutacji(int opcja) { metodaMutacji = opcja; }
    void ustawLimitCzasu(double sekundy) { limitCzasu = sekundy; }

    bool wczytajZPliku(const string& nazwaPliku) {
        ifstream plik(nazwaPliku);
        if (!plik.is_open()) return false;

        string linia;
        liczbaMiast = 0;

        // Szukanie rozmiaru instancji w naglowku pliku
        while (getline(plik, linia)) {
            if (linia.find("DIMENSION") != string::npos) {
                size_t dwukropek = linia.find(":");
                if (dwukropek != string::npos) {
                    liczbaMiast = stoi(linia.substr(dwukropek + 1));
                }
                else {
                    stringstream ss(linia);
                    string tmp;
                    ss >> tmp >> liczbaMiast;
                }
                break;
            }
        }

        // Przejdz do sekcji danych macierzy
        while (getline(plik, linia)) {
            if (linia.find("EDGE_WEIGHT_SECTION") != string::npos) {
                break;
            }
        }

        if (liczbaMiast == 0) return false;

        // Alokacja i wczytywanie pelnej macierzy kosztow
        macierzKosztow.assign(liczbaMiast, vector<int>(liczbaMiast, 0));
        for (int i = 0; i < liczbaMiast; ++i) {
            for (int j = 0; j < liczbaMiast; ++j) {
                plik >> macierzKosztow[i][j];
            }
        }

        plik.close();
        return true;
    }

    void wyswietlDane() {
        if (liczbaMiast == 0) {
            cout << "Brak wczytanych danych!" << endl;
            return;
        }
        cout << "Rozmiar wczytanej instancji: " << liczbaMiast << " miast." << endl;
        if (liczbaMiast <= 15) { // Wyswietl tylko dla malych instancji, zeby nie zapchac konsoli
            for (int i = 0; i < liczbaMiast; ++i) {
                for (int j = 0; j < liczbaMiast; ++j) {
                    cout << setw(5) << macierzKosztow[i][j] << " ";
                }
                cout << endl;
            }
        }
        else {
            cout << "[Podglad ukryty - macierz jest zbyt duza do wyswietlenia]" << endl;
        }
    }

    // SELEKCJA: Metoda Turniejowa (Rozmiar turnieju = 3)
    Osobnik selekcjaTurniejowa(const vector<Osobnik>& obecnaPopulacja) {
        uniform_int_distribution<int> dist(0, rozmiarPopulacji - 1);
        int najlepszyIdx = dist(rng);

        // Losujemy pozostalych uczestnikow turnieju
        for (int i = 0; i < 2; ++i) {
            int losowyIdx = dist(rng);
            if (obecnaPopulacja[losowyIdx].fitness < obecnaPopulacja[najlepszyIdx].fitness) {
                najlepszyIdx = losowyIdx;
            }
        }
        return obecnaPopulacja[najlepszyIdx];
    }

    // KRZYZOWANIE: Order Crossover (OX)
    Osobnik krzyzowanieOX(const Osobnik& rodzic1, const Osobnik& rodzic2) {
        Osobnik dziecko;
        dziecko.trasa.assign(liczbaMiast, -1);

        uniform_int_distribution<int> dist(0, liczbaMiast - 1);
        int punkt1 = dist(rng);
        int punkt2 = dist(rng);

        if (punkt1 > punkt2) swap(punkt1, punkt2);

        // 1. Przepisujemy wycinek od pierwszego rodzica
        for (int i = punkt1; i <= punkt2; ++i) {
            dziecko.trasa[i] = rodzic1.trasa[i];
        }

        // 2. Uzupelniamy puste miejsca genami drugiego rodzica zachowujac kolejnosc
        int aktualnyIdxDziecka = (punkt2 + 1) % liczbaMiast;
        int aktualnyIdxRodzica2 = (punkt2 + 1) % liczbaMiast;

        for (int i = 0; i < liczbaMiast; ++i) {
            int gen = rodzic2.trasa[aktualnyIdxRodzica2];

            // Sprawdzamy, czy dziecko ma juz ten gen
            bool juzIstnieje = false;
            for (int j = punkt1; j <= punkt2; ++j) {
                if (dziecko.trasa[j] == gen) {
                    juzIstnieje = true;
                    break;
                }
            }

            if (!juzIstnieje) {
                dziecko.trasa[aktualnyIdxDziecka] = gen;
                aktualnyIdxDziecka = (aktualnyIdxDziecka + 1) % liczbaMiast;
            }
            aktualnyIdxRodzica2 = (aktualnyIdxRodzica2 + 1) % liczbaMiast;
        }

        dziecko.fitness = obliczKosztTrasy(dziecko.trasa);
        return dziecko;
    }

    // MUTACJA 1: Swap Mutation (Zamiana miejscami)
    void mutacjaSwap(Osobnik& osobnik) {
        uniform_int_distribution<int> dist(0, liczbaMiast - 1);
        int i = dist(rng);
        int j = dist(rng);
        while (i == j) j = dist(rng);

        swap(osobnik.trasa[i], osobnik.trasa[j]);
        osobnik.fitness = obliczKosztTrasy(osobnik.trasa);
    }

    // MUTACJA 2: Inversion Mutation (Odwracanie podciagu)
    void mutacjaInwersji(Osobnik& osobnik) {
        uniform_int_distribution<int> dist(0, liczbaMiast - 1);
        int i = dist(rng);
        int j = dist(rng);
        if (i > j) swap(i, j);

        // Odwracamy kolejnosc elementow w wybranym przedziale [i, j]
        reverse(osobnik.trasa.begin() + i, osobnik.trasa.begin() + j + 1);
        osobnik.fitness = obliczKosztTrasy(osobnik.trasa);
    }

    // GLOWNA PROCEDURA URUCHOMIENIOWA
    void uruchom(int optymalnyKoszt) {
        if (liczbaMiast == 0) {
            cout << "Blad: Najpierw wczytaj dane instancji!" << endl;
            return;
        }

        // 1. Inicjalizacja populacji poczatkowej (Losowej)
        vector<Osobnik> populacja(rozmiarPopulacji);
        vector<int> bazowaTrasa(liczbaMiast);
        for (int i = 0; i < liczbaMiast; ++i) bazowaTrasa[i] = i;

        Osobnik najlepszyGlobalnie;
        najlepszyGlobalnie.fitness = 1e9; // Nieskonczonosc na start

        for (int i = 0; i < rozmiarPopulacji; ++i) {
            populacja[i].trasa = bazowaTrasa;
            shuffle(populacja[i].trasa.begin(), populacja[i].trasa.end(), rng);
            populacja[i].fitness = obliczKosztTrasy(populacja[i].trasa);

            if (populacja[i].fitness < najlepszyGlobalnie.fitness) {
                najlepszyGlobalnie = populacja[i];
            }
        }

        // Przygotowanie rozkladow prawdopodobienstwa dla krzyzowania/mutacji
        uniform_real_distribution<double> randProb(0.0, 1.0);

        // Start pomiaru czasu
        auto czasStart = chrono::high_resolution_clock::now();
        long long iteracja = 0;

        while (true) {
            // Sprawdzenie kryterium stopu (Czasowe)
            auto czasTeraz = chrono::high_resolution_clock::now();
            chrono::duration<double> elaps = czasTeraz - czasStart;
            if (elaps.count() >= limitCzasu) break;

            vector<Osobnik> nowaPopulacja;

            // Tworzenie nowego pokolenia
            while (nowaPopulacja.size() < (size_t)rozmiarPopulacji) {
                // Selekcja rodzicow
                Osobnik rodzic1 = selekcjaTurniejowa(populacja);
                Osobnik rodzic2 = selekcjaTurniejowa(populacja);

                Osobnik potomek1 = rodzic1;
                Osobnik potomek2 = rodzic2;

                // Krzyzowanie z zadanym prawdopodobienstwem
                if (randProb(rng) < wspolczynnikKrzyzowania) {
                    potomek1 = krzyzowanieOX(rodzic1, rodzic2);
                    potomek2 = krzyzowanieOX(rodzic2, rodzic1);
                }

                // Mutacja z zadanym prawdopodobienstwem
                if (randProb(rng) < wspolczynnikMutacji) {
                    if (metodaMutacji == 1) mutacjaSwap(potomek1);
                    else mutacjaInwersji(potomek1);
                }
                if (randProb(rng) < wspolczynnikMutacji) {
                    if (metodaMutacji == 1) mutacjaSwap(potomek2);
                    else mutacjaInwersji(potomek2);
                }

                nowaPopulacja.push_back(potomek1);
                if (nowaPopulacja.size() < (size_t)rozmiarPopulacji) {
                    nowaPopulacja.push_back(potomek2);
                }
            }

            populacja = nowaPopulacja;

            // Aktualizacja rekordu globalnego i szukanie najlepszego w danej generacji
            for (int i = 0; i < rozmiarPopulacji; ++i) {
                if (populacja[i].fitness < najlepszyGlobalnie.fitness) {
                    najlepszyGlobalnie = populacja[i];
                }
            }
            iteracja++;
        }

        // Obliczanie sredniej z ostatniej populacji
        long long sumaKosztow = 0;
        for (int i = 0; i < rozmiarPopulacji; ++i) {
            sumaKosztow += populacja[i].fitness;
        }
        double sredniKosztOstatniej = (double)sumaKosztow / rozmiarPopulacji;

        // Obliczanie bledow wzglednych (jezeli uzytkownik podal optymalny koszt)
        double bladNajlepszy = 0.0;
        double bladSredni = 0.0;
        if (optymalnyKoszt > 0) {
            bladNajlepszy = ((double)(najlepszyGlobalnie.fitness - optymalnyKoszt) / optymalnyKoszt) * 100.0;
            bladSredni = ((sredniKosztOstatniej - optymalnyKoszt) / optymalnyKoszt) * 100.0;
        }

        // WYSWIETLENIE WYNIKOW (Zgodnie z uwagami z PDF)
        cout << "\n================ WYNIKI ALGORYTMU ================" << endl;
        cout << "Przeanalizowano pokolen: " << iteracja << endl;
        cout << "Najlepsze znalezione rozwiazanie (koszt): " << najlepszyGlobalnie.fitness << endl;
        cout << "Srednia dlugosc tras w ostatniej populacji: " << fixed << setprecision(2) << sredniKosztOstatniej << endl;

        if (optymalnyKoszt > 0) {
            cout << "Bw najlepszego rozwiazania: " << bladNajlepszy << "%" << endl;
            cout << "Bw sredniej dlugosci ostatniej populacji: " << bladSredni << "%" << endl;
        }
        else {
            cout << "[Wskazowka: Podaj optymalna wartosc w menu, aby wyliczyc bledy wzgledne]" << endl;
        }

        cout << "Najlepsza trasa: ";
        for (int i = 0; i < liczbaMiast; ++i) cout << najlepszyGlobalnie.trasa[i] << " -> ";
        cout << najlepszyGlobalnie.trasa[0] << endl;
        cout << "==================================================" << endl;
    }
};

int main() {
    AlgorytmGenetyczny ga;
    int opcja;
    string plik;
    double limitSekund = 10.0;
    int popRozmiar, metMut;
    double wspMut, wspKrzyz;
    int znaneOptimum = -1; // Uzywane do automatycznego liczenia bledu wzglendnego

    do {
        cout << "\n=== PEA PROJEKT 4 - ALGORYTM GENETYCZNY ===" << endl;
        cout << "1. Wczytaj dane z pliku i wyswietl" << endl;
        cout << "2. Wprowadz kryterium stopu (czas w sekundach) [Aktualnie: " << limitSekund << "s]" << endl;
        cout << "3. Ustaw wielkosc populacji poczatkowej" << endl;
        cout << "4. Ustaw wspolczynnik mutacji" << endl;
        cout << "5. Ustaw wspolczynnik krzyzowania" << endl;
        cout << "6. Wybor metody mutacji (1-Swap, 2-Inwersja)" << endl;
        cout << "7. [INFO] Wybor selekcji/krzyzowania" << endl;
        cout << "8. Uruchom algorytm i wyswietl wyniki" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        cin >> opcja;

        switch (opcja) {
        case 1:
            cout << "Podaj nazwe pliku z danymi (np. ftv170.atsp): ";
            cin >> plik;
            if (ga.wczytajZPliku(plik)) {
                cout << "[OK] Dane zaladowane!" << endl;
                ga.wyswietlDane();
                // Automatyczne podpowiedzi optimum dla ulatwienia testow studenta
                if (plik.find("ftv170") != string::npos) znaneOptimum = 2755;
                else if (plik.find("ft53") != string::npos) znaneOptimum = 6905;
                else {
                    cout << "Podaj znane optimum globalne dla tego pliku (lub 0 jesli nie znasz): ";
                    cin >> znaneOptimum;
                }
            }
            else {
                cout << "[BLAD] Nie udalo sie otworzyc pliku!" << endl;
            }
            break;
        case 2:
            cout << "Podaj limit czasu (sekundy): ";
            cin >> limitSekund;
            ga.ustawLimitCzasu(limitSekund);
            break;
        case 3:
            cout << "Podaj wielkosc populacji (np. 100): ";
            cin >> popRozmiar;
            ga.ustawRozmiarPopulacji(popRozmiar);
            break;
        case 4:
            cout << "Podaj wspolczynnik mutacji (0.01 - 0.10): ";
            cin >> wspMut;
            ga.ustawWspolczynnikMutacji(wspMut);
            break;
        case 5:
            cout << "Podaj wspolczynnik krzyzowania (0.50 - 0.95): ";
            cin >> wspKrzyz;
            ga.ustawWspolczynnikKrzyzowania(wspKrzyz);
            break;
        case 6:
            cout << "Wybierz metode mutacji:\n1 - Swap Mutation\n2 - Inversion Mutation\nWybor: ";
            cin >> metMut;
            if (metMut == 1 || metMut == 2) ga.ustawMetodeMutacji(metMut);
            else cout << "Blad wyboru!" << endl;
            break;
        case 7:
            cout << "[Wariant 4.0]: Na sztywno ustawiono Turniej oraz Krzyzowanie OX." << endl;
            break;
        case 8:
            ga.uruchom(znaneOptimum);
            break;
        }
    } while (opcja != 0);

    return 0;
}