#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// Recursive function to calculate binomial coefficient
long binomialCoeffRecursive(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    }
    if (k > n) {
        return 0;
    }
    return binomialCoeffRecursive(n - 1, k - 1) + binomialCoeffRecursive(n - 1, k);
}

// Dynamic programming function to calculate binomial coefficient
long binomialCoeffDP(int n, int k) {
    vector<vector<long>> C(n + 1, vector<long>(k + 1, 0));

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= min(i, k); j++) {
            if (j == 0 || j == i) {
                C[i][j] = 1;
            } else {
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            }
        }
    }
    return C[n][k];
}

int main() {
    int n, k;
    cout << "Enter values for n and k: ";
    cin >> n >> k;

    auto start = high_resolution_clock::now();
    int resultRecursive = binomialCoeffRecursive(n, k);
    auto stop = high_resolution_clock::now();
    auto durationRecursive = duration_cast<microseconds>(stop - start);
    cout << "Recursive result: " << resultRecursive << " (Time taken: " << durationRecursive.count() << " microseconds)" << endl;

    start = high_resolution_clock::now();
    int resultDP = binomialCoeffDP(n, k);
    stop = high_resolution_clock::now();
    auto durationDP = duration_cast<microseconds>(stop - start);
    cout << "DP result: " << resultDP << " (Time taken: " << durationDP.count() << " microseconds)" << endl;

    return 0;
}