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

inline int ReduceMatrix(vector<vector<int>>& matrix){
    int n = matrix.size();
    int reductionCost = 0;

	// redukcja wierszy
    for (int i = 0; i < n; ++i){
        int min_val = INT_MAX;
        for (int j = 0; j < n; ++j){
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