# Projektowanie Efektywnych Algorytmów (PEA)

Repozytorium zawiera rozwiązania projektów akademickich z przedmiotu Projektowanie Efektywnych Algorytmów. Głównym problemem optymalizacyjnym badanym we wszystkich zadaniach jest **Asymetryczny Problem Komiwojażera (ATSP)**. 

Celem projektów była implementacja różnych podejść algorytmicznych, zbadanie ich złożoności obliczeniowej oraz analiza kompromisu między czasem działania a jakością znalezionego rozwiązania (tzw. *trade-off*).

## 📂 Zawartość repozytorium

* **PEA Zadanie 1 - Algorytmy Dokładne**
  Implementacja algorytmów gwarantujących znalezienie optymalnego rozwiązania:
  * Przegląd zupełny (Brute Force)
  * Metoda podziału i ograniczeń (Branch and Bound)
  
* **PEA Zadanie 2 - Heurystyki i Przeszukiwanie z Zakazami**
  Zastosowanie metod przybliżonych do szybszego znajdowania suboptymalnych tras:
  * Algorytm zachłanny (Nearest Neighbor)
  * Metaheurystyka Tabu Search (z uwzględnieniem listy zakazów i aspiracji)

* **PEA Zadanie 3 - Symulowane Wyżarzanie**
  Implementacja metaheurystyki inspirowanej procesem chłodzenia w metalurgii:
  * Algorytm Symulowanego Wyżarzania (Simulated Annealing)
  * Badanie wpływu parametrów chłodzenia i trasy startowej na ostateczny wynik.

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
