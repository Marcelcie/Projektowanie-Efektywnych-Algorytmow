#pragma once
#include <vector>
#include <iostream>
#include <climits>

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
        bool empty() const {
            return heap.empty();
        }
        inline int SolveTSP(vector<vector<int>> initialization_matrix) {
            int n = initialization_matrix.size();
            //poczekalnia
            MinHeap pq;
			//Tworzenie węzła początkowego
            Node root;
			root.matrix = initialization_matrix;
            root.path.push_back(0);
            root.city = 0;
            root.level = 0;
			root.lowerBound = ReduceMatrix(root.matrix); // liczymy bazowy koszt startowy

            pq.push(root);

			// zmienna przechowująca koszt najlepszego rozwiązanie
            int minTourCost = INT_MAX;

			//główna pętla

            while (!pq.isEmpty() {
                Node curr = pq.pop();
                //bound
                if (curr.lowerBound > minTourCost){
                continue        
                }
                int i curr.city;
                if (curr.level == n - 1) {
                    // Jeśli tak, aktualizujemy nasz najlepszy wynik i kończymy tę gałąź
                    if (curr.lowerBound < minTourCost) {
                        minTourCost = curr.lowerBound;
                    }
                    continue;
                }
            }   




}
};