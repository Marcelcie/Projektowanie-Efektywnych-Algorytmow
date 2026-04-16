#pragma once
#include <vector>
#include <iostream>
#include <climits>
#include <chrono>

using namespace std;

struct Node {
    vector<vector<int>> matrix;
    vector<int> path;
    int lowerBound;
    int city;
    int level;
};
// funkcja redukująca macierz i zwracająca koszt redukcji
inline int ReduceMatrix(vector<vector<int>>& matrix){
    int n = matrix.size();
    int reductionCost = 0;

	// redukcja wierszy
    for (int i = 0; i < n; ++i) {
        int min_val = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] != -1 && matrix[i][j] < min_val) {
                min_val = matrix[i][j];
            }
        }

        if (min_val != INT_MAX && min_val > 0) {
            reductionCost += min_val;
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] != -1) {
                    matrix[i][j] -= min_val;
                }
            }
        }
    }
		// redukcja kolumn
        for (int j = 0; j < n; ++j) {
			int min_val = INT_MAX;
            for (int i = 0; i < n; ++i) {
                if (matrix[i][j] != -1 && matrix[i][j] < min_val) {
                    min_val = matrix[i][j];
                }
            }
            if (min_val !=  INT_MAX && min_val > 0) {
				reductionCost += min_val;
                for (int i=0; i < n; ++i) {
                    if (matrix[i][j] != -1) {
                        matrix[i][j] -= min_val;
                    }
				}
            }
        }
        return reductionCost;
}

//kopiec (min-heap)
class MinHeap {
private:
    vector<Node> heap;
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].lowerBound < heap[parent].lowerBound) {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else {
                break;
            }
        }
    }
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;
            if (left < size && heap[left].lowerBound < heap[smallest].lowerBound) {
                smallest = left;
            }
            if (right < size && heap[right].lowerBound < heap[smallest].lowerBound) {
                smallest = right;
            }
            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else {
                break;
            };

        }
    }
public:
    void push(Node node) {
        heap.push_back(node);
        heapifyUp(heap.size() - 1);
    }
    Node pop() {
        Node bestNode = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return bestNode;
    }
    bool isEmpty() const {
        return heap.empty();
    }
};

// 3. Główny algorytm: BRANCH AND BOUND (TSP)

inline int SolveTSP(vector<vector<int>> initialization_matrix, int initial_upper_bound = INT_MAX) {
    int n = initialization_matrix.size();
    MinHeap pq;
    // Tworzenie węzła początkowego
    Node root;
    root.matrix = initialization_matrix;
    root.path.push_back(0);
    root.city = 0;
    root.level = 0;
    root.lowerBound = ReduceMatrix(root.matrix);

    pq.push(root);

    int minTourCost = initial_upper_bound;

    auto start_time = chrono::high_resolution_clock::now(); //uruchamiamy stoper 
    // Główna pętla
    while (!pq.isEmpty()) {

        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::seconds>(current_time - start_time).count() > 300) {
            cout << "\nPrzekroczono limit 5 minut!.\n";
            return minTourCost;
        }

        Node curr = pq.pop();

        // BOUND
        if (curr.lowerBound >= minTourCost) {
            continue;
        }

        int i = curr.city;

        if (curr.level == n - 1) {
            if (curr.lowerBound < minTourCost) {
                minTourCost = curr.lowerBound;
            }
            continue;
        }

        // BRANCH - Tego kodu Ci brakowało! Generowanie nowych tras
        for (int j = 0; j < n; j++) {
            if (curr.matrix[i][j] != -1) {

                Node child;
                child.path = curr.path;
                child.path.push_back(j);
                child.level = curr.level + 1;
                child.city = j;
                child.matrix = curr.matrix;

                for (int k = 0; k < n; k++) {
                    child.matrix[i][k] = -1;
                    child.matrix[k][j] = -1;
                }
                child.matrix[j][0] = -1;

                int costOfEdge = curr.matrix[i][j];
                int reducedCost = ReduceMatrix(child.matrix);

                child.lowerBound = curr.lowerBound + costOfEdge + reducedCost;

                pq.push(child);
            }
        }
    }
    return minTourCost; // Funkcja w końcu zwraca wynik!
}

//kolejka FIFO
class MyQueue {
private:
	vector<Node> q;
public:
    void push(Node node) {
		q.push_back(node); // dodajemy element na koniec kolejki
    }
    Node pop() {
        Node firstNode = q[0];
		q.erase(q.begin()); // usuwamy pierwszy element z kolejki
		return firstNode;        
    }
    bool isEmpty() const {
        return q.empty();
    }

};

// 4. Algorytm: BRANCH AND BOUND (Breadth-First Search - Przeszukiwanie Wszerz)

inline int SolveTSPBreadth(vector<vector<int>> initialization_matrix, int initial_upper_bound = INT_MAX) {
    int n = initialization_matrix.size();

    // Używamy Twojej nowej kolejki FIFO zamiast Kopca!
    MyQueue pq;

    // Tworzenie węzła początkowego
    Node root;
    root.matrix = initialization_matrix;
    root.path.push_back(0);
    root.city = 0;
    root.level = 0;
    root.lowerBound = ReduceMatrix(root.matrix);

    pq.push(root);

    int minTourCost = initial_upper_bound;

    // Główna pętla
    auto start_time = chrono::high_resolution_clock::now();
    while (!pq.isEmpty()) {

        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::seconds>(current_time - start_time).count() > 300) {
            cout << "\n[TIMEOUT] Przekroczono limit 5 minut! Przerwano obliczenia.\n";
            return minTourCost;
        }

        Node curr = pq.pop();

        // BOUND
        if (curr.lowerBound >= minTourCost) {
            continue;
        }

        int i = curr.city;

        if (curr.level == n - 1) {
            if (curr.lowerBound < minTourCost) {
                minTourCost = curr.lowerBound;
            }
            continue;
        }

        // BRANCH 
        for (int j = 0; j < n; j++) {
            if (curr.matrix[i][j] != -1) {

                Node child;
                child.path = curr.path;
                child.path.push_back(j);
                child.level = curr.level + 1;
                child.city = j;
                child.matrix = curr.matrix;

                for (int k = 0; k < n; k++) {
                    child.matrix[i][k] = -1;
                    child.matrix[k][j] = -1;
                }
                child.matrix[j][0] = -1;

                int costOfEdge = curr.matrix[i][j];
                int reducedCost = ReduceMatrix(child.matrix);

                child.lowerBound = curr.lowerBound + costOfEdge + reducedCost;

                pq.push(child);
            }
        }
    }
    return minTourCost;
}