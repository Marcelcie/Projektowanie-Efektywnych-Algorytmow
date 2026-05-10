#pragma once
#include <vector>

enum CoolingSchedule {
    GEOMETRIC,
    LINEAR
};

enum InitialSolutionType {
    RANDOM_PATH,
    GREEDY_PATH
};

class SimulatedAnnealing {
private:
    std::vector<std::vector<int>> matrix;
    int N;

    double T_start;
    double T_end;
    double alpha;
    int epochLength;
    double timeLimitSeconds;

    CoolingSchedule schedule;
    InitialSolutionType initialSolution;

    std::vector<int> bestPath;
    long long bestCost;

    long long CalculatePathCost(const std::vector<int>& path);
    std::vector<int> GenerateRandomPath();
    std::vector<int> GenerateGreedyPath();
    double CoolTemperature(double currentT);

public:
    SimulatedAnnealing(const std::vector<std::vector<int>>& m);

    void SetTimeLimit(double limit);
    void SetParameters(double t_start, double a, int epoch, CoolingSchedule sched, InitialSolutionType initSol);

    void Run();

    long long GetBestCost() const { return bestCost; }
    std::vector<int> GetBestPath() const { return bestPath; }
};