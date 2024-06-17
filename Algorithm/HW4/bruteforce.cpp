#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip> // Include for setting precision

using namespace std;
using namespace std::chrono;

struct Item {
    int weight;
    int value;
};

vector<Item> generateItems(int n, int seed) {
    vector<Item> items(n);
    mt19937 gen(seed);
    uniform_int_distribution<> disValue(1, 500);
    uniform_int_distribution<> disWeight(1, 100);
    
    for (int i = 0; i < n; i++) {
        items[i].value = disValue(gen);
        items[i].weight = disWeight(gen);
    }
    return items;
}

long long bruteForceKnapsack(const vector<Item>& items, int W) {
    int n = items.size();
    long long maxVal = 0;
    unsigned long long numOfCombinations = 1ULL << n;

    long long totalCombinations = 1LL << n; 
    for (unsigned long long i = 0; i < numOfCombinations; i++) {
        int currentWeight = 0, currentValue = 0;
        for (int j = 0; j < n; j++) {
            if (i & (1ULL << j)) {
                currentWeight += items[j].weight;
                currentValue += items[j].value;
            }
        }
        if (currentWeight <= W && currentValue > maxVal) {
            maxVal = currentValue;
        }
    }
    return maxVal;
}

int main() {
    int seed = 100;
    int sizes[] = {11, 21, 31};

    cout << fixed << setprecision(3); // Set the precision for floating-point output
    cout << "[1] Brute Force" << endl;
    cout << "Number of Items  /  Processing time(ms)  /  Maximum Benefit Value" << endl;

    for (int n : sizes) {
        int W = n * 25;
        auto items = generateItems(n, seed);
        
        auto start = high_resolution_clock::now();
        int maxVal = bruteForceKnapsack(items, W);
        auto stop = high_resolution_clock::now();

        duration<double, milli> duration = stop - start; // Explicitly defining the type

        cout << n << "  /  " << duration.count() << "ms" << "  /  " << maxVal << endl;
    }
    
    return 0;
}