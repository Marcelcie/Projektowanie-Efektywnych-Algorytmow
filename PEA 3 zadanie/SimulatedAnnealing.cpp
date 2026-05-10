#include "SimulatedAnnealing.h"
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <iostream>

const long long INF = 9999999999; // Zabezpieczenie przed brakiem ścieżki

SimulatedAnnealing::SimulatedAnnealing(const std::vector<std::vector<int>>& m) {
    matrix = m;
    N = matrix.size();
    T_start = 10000.0;
    T_end = 0.0001;
    alpha = 0.99;
    epochLength = N * 10;
    timeLimitSeconds = 60.0;
    schedule = GEOMETRIC;
    initialSolution = RANDOM_PATH;
}

void SimulatedAnnealing::SetTimeLimit(double limit) {
    timeLimitSeconds = limit;
}

void SimulatedAnnealing::SetParameters(double t_start, double a, int epoch, CoolingSchedule sched, InitialSolutionType initSol) {
    T_start = t_start;
    alpha = a;
    epochLength = epoch;
    schedule = sched;
    initialSolution = initSol;
}

long long SimulatedAnnealing::CalculatePathCost(const std::vector<int>& path) {
    long long cost = 0;
    for (int i = 0; i < N - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        if (matrix[u][v] == -1) return INF;
        cost += matrix[u][v];
    }
    int last = path[N - 1];
    int first = path[0];
    if (matrix[last][first] == -1) return INF;
    cost += matrix[last][first];
    return cost;
}

std::vector<int> SimulatedAnnealing::GenerateRandomPath() {
    std::vector<int> path(N);
    std::iota(path.begin(), path.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(path.begin(), path.end(), g);
    return path;
}

std::vector<int> SimulatedAnnealing::GenerateGreedyPath() {
    std::vector<int> path;
    std::vector<bool> visited(N, false);
    int currentCity = 0;
    path.push_back(currentCity);
    visited[currentCity] = true;

    for (int i = 1; i < N; ++i) {
        int nextCity = -1;
        int minCost = 2147483647;
        for (int j = 0; j < N; ++j) {
            if (!visited[j] && matrix[currentCity][j] < minCost && matrix[currentCity][j] != -1) {
                minCost = matrix[currentCity][j];
                nextCity = j;
            }
        }
        if (nextCity == -1) {
            for (int j = 0; j < N; ++j) if (!visited[j]) { nextCity = j; break; }
        }
        currentCity = nextCity;
        path.push_back(currentCity);
        visited[currentCity] = true;
    }
    return path;
}

double SimulatedAnnealing::CoolTemperature(double currentT) {
    if (schedule == GEOMETRIC) return currentT * alpha;
    else return (currentT - alpha > 0) ? (currentT - alpha) : 0.0001;
}

void SimulatedAnnealing::Run() {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> currentPath;
    if (initialSolution == RANDOM_PATH) currentPath = GenerateRandomPath();
    else currentPath = GenerateGreedyPath();

    long long currentCost = CalculatePathCost(currentPath);
    bestPath = currentPath;
    bestCost = currentCost;

    double T = T_start;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);
    std::uniform_int_distribution<> city_dist(0, N - 1);

    while (T > T_end) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        if (elapsed.count() >= timeLimitSeconds) break;

        for (int i = 0; i < epochLength; ++i) {
            std::vector<int> nextPath = currentPath;

            int c1 = city_dist(gen);
            int c2 = city_dist(gen);
            while (c1 == c2) { c2 = city_dist(gen); }
            std::swap(nextPath[c1], nextPath[c2]);

            long long nextCost = CalculatePathCost(nextPath);
            long long deltaE = nextCost - currentCost;

            if (deltaE < 0) {
                currentPath = nextPath;
                currentCost = nextCost;
                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    bestPath = currentPath;
                }
            }
            else {
                double probability = exp(-deltaE / T);
                if (probability > prob_dist(gen)) {
                    currentPath = nextPath;
                    currentCost = nextCost;
                }
            }
        }
        T = CoolTemperature(T);
    }
}