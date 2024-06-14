#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>  
#include <ctime>    

using namespace std;

// Hàm t?o m?t cá th? ng?u nhiên
vector<int> createIndividual(int n) {
    vector<int> individual(n);
    for (int i = 0; i < n; ++i) {
        individual[i] = i + 1;
    }
    random_shuffle(individual.begin(), individual.end());
    return individual;
}

// Hàm tính giá tr? fitness c?a m?t cá th?
int calculateFitness(const vector<int>& individual, const vector<vector<int>>& distanceMatrix) {
    int fitness = 0;
    for (int i = 0; i < individual.size() - 1; ++i) {
        fitness += distanceMatrix[individual[i] - 1][individual[i + 1] - 1];
    }
    fitness += distanceMatrix[individual.back() - 1][individual[0] - 1];  // Quay l?i d?nh d?u tiên
    return fitness;
}

// Hàm lai ghép gi?a hai cá th?
vector<int> crossover(const vector<int>& parent1, const vector<int>& parent2) {
    int crossoverPoint = rand() % (parent1.size() - 1) + 1;
    vector<int> child(parent1.begin(), parent1.begin() + crossoverPoint);
    for (int gene : parent2) {
        if (find(child.begin(), child.end(), gene) == child.end()) {
            child.push_back(gene);
        }
    }
    return child;
}

// Hàm d?t bi?n trên m?t cá th?
void mutate(vector<int>& individual) {
    int mutationPoint1 = rand() % individual.size();
    int mutationPoint2 = rand() % individual.size();
    swap(individual[mutationPoint1], individual[mutationPoint2]);
}

// Thu?t toán di truy?n
pair<vector<int>, int> geneticAlgorithm(const vector<vector<int>>& distanceMatrix, int populationSize = 50, int generations = 1000) {
    int n = distanceMatrix.size();
    vector<vector<int>> population(populationSize);

    for (int i = 0; i < populationSize; ++i) {
        population[i] = createIndividual(n);
    }

    for (int generation = 0; generation < generations; ++generation) {
        sort(population.begin(), population.end(), [&distanceMatrix](const vector<int>& a, const vector<int>& b) {
            return calculateFitness(a, distanceMatrix) < calculateFitness(b, distanceMatrix);
        });

        vector<vector<int>> newPopulation;

        for (int i = 0; i < populationSize; i += 2) {
            const vector<int>& parent1 = population[i];
            const vector<int>& parent2 = population[i + 1];
            vector<int> child1 = crossover(parent1, parent2);
            vector<int> child2 = crossover(parent2, parent1);
            mutate(child1);
            mutate(child2);
            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }

        population = newPopulation;
    }

    vector<int> bestSolution = *min_element(population.begin(), population.end(), [&distanceMatrix](const vector<int>& a, const vector<int>& b) {
        return calculateFitness(a, distanceMatrix) < calculateFitness(b, distanceMatrix);
    });

    int bestFitness = calculateFitness(bestSolution, distanceMatrix);

    return make_pair(bestSolution, bestFitness);
}

// Hàm d?c ma tr?n kho?ng cách t? file input
pair<int, vector<vector<int>>> readInput(const string& filePath) {
    ifstream file(filePath);
    int n;
    file >> n;
    vector<vector<int>> distanceMatrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> distanceMatrix[i][j];
        }
    }
    return make_pair(n, distanceMatrix);
}

// Hàm ghi k?t qu? ra file output
void writeOutput(const string& filePath, int n, const vector<int>& bestSolution, int bestFitness) {
    ofstream file(filePath);
    file << n << " " << bestFitness << endl;
    for (int gene : bestSolution) {
        file << gene << " ";
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    string inputFile = "tsp4.txt";
    string outputFile = "tsp4_out.txt";

    pair<int, vector<vector<int>>> input = readInput(inputFile);
    int inputSize = input.first;
    vector<vector<int>> distanceMatrix = input.second;

    pair<vector<int>, int> result = geneticAlgorithm(distanceMatrix);
    vector<int> bestSolution = result.first;
    int bestFitness = result.second;

    writeOutput(outputFile, inputSize, bestSolution, bestFitness);

    return 0;
}
