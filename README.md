# Projektowanie Efektywnych Algorytmów (PEA)

Repozytorium zawiera rozwiązania projektów akademickich z przedmiotu Projektowanie Efektywnych Algorytmów. Głównym problemem optymalizacyjnym badanym we wszystkich zadaniach jest **Asymetryczny Problem Komiwojażera (ATSP)**. 

Celem projektów była implementacja różnych podejść algorytmicznych, zbadanie ich złożoności obliczeniowej oraz analiza kompromisu między czasem działania a jakością znalezionego rozwiązania (tzw. *trade-off*).

## 📂 Zawartość repozytorium

* **PEA Projekt 1 - Algorytmy Zachłanne i Przegląd Zupełny**
  Implementacja podstawowych podejść dokładnych oraz heurystyk startowych weryfikująca różnice w czasie i błędzie względnym:
  * Przegląd zupełny (Brute Force - BF)
  * Algorytm zachłanny (Nearest Neighbor - NN)
  * Rozszerzony algorytm zachłanny (Repetitive Nearest Neighbor - RNN)
  * Przeszukiwanie losowe (Random Search - RS)
  
* **PEA Projekt 2 - Metoda Podziału i Ograniczeń (Branch and Bound)**
  Implementacja algorytmu dokładnego optymalizującego przeszukiwanie drzewa stanów z wykorzystaniem dynamicznie wyliczanego budżetu startowego:
  * Strategia Best-First Search (z wykorzystaniem kolejki priorytetowej / kopca)
  * Strategia Breadth-First Search (z wykorzystaniem kolejki FIFO)

* **PEA Zadanie 3 - Symulowane Wyżarzanie**
  Implementacja metaheurystyki inspirowanej procesem chłodzenia w metalurgii:
  * Algorytm Symulowanego Wyżarzania (Simulated Annealing)
  * Badanie wpływu parametrów chłodzenia i trasy startowej na ostateczny wynik.
 
* **PEA Zadanie 4 - Algorytm Genetyczny**
  Implementacja heurystycznego algorytmu ewolucyjnego opartego na populacji rozwiązań permutacyjnych dla problemu ATSP:
  * Selekcja: Turniejowa (z domyślnym rozmiarem turnieju równym 3)
  * Krzyżowanie: Order Crossover (OX) zachowujące prawidłową kolejność miast
  * Mutacja: Dwa alternatywne warianty zaburzenia chromosomu – Mutacja Zamiany (Swap) oraz Mutacja Inwersji (Inversion)
  * Kryterium stopu: Elastyczny limit czasowy działania wyrażony w sekundach

## 🛠️ Technologie
* **Język:** C++ (Standard C++11/14+)
* **Środowisko:** Visual Studio 2022
* **Struktury danych:** Macierze sąsiedztwa, wektory (`std::vector`)

## 🚀 Uruchomienie
Wszystkie projekty zostały przygotowane w środowisku Visual Studio. Aby uruchomić wybrany algorytm:
1. Sklonuj repozytorium na swój dysk.
2. Otwórz plik `.sln` (Solution) z folderu konkretnego zadania w Visual Studio.
3. Skompiluj projekt w trybie **Release** (dla maksymalnej wydajności i rzetelnych pomiarów czasu).
4. Uruchom program i wczytaj odpowiednią instancję testową (np. z formatu TSPLIB).
