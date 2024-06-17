#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <queue>
#include <random>
#include <iomanip> // Include for setting precision

using namespace std;
using namespace chrono;

struct Item {
    int benefit;
    int weight;
};

vector<Item> generateItems(int n, mt19937& rng) {
    uniform_int_distribution<int> benefitDist(1, 500);
    uniform_int_distribution<int> weightDist(1, 100);

    vector<Item> items(n);
    for (auto& item : items) {
        item.benefit = benefitDist(rng);
        item.weight = weightDist(rng);
    }
    return items;
}

bool compare(const Item& a, const Item& b) {
    double r1 = (double)a.benefit / a.weight;
    double r2 = (double)b.benefit / b.weight;
    return r1 > r2;
}

int dpKnapsack(const vector<Item>& items, int W) {
    int n = items.size();
    vector<vector<int>> dp(n+1, vector<int>(W+1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (items[i-1].weight <= w) {
                dp[i][w] = max(dp[i-1][w], dp[i-1][w-items[i-1].weight] + items[i-1].benefit);
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    return dp[n][W];
}

int greedyKnapsack(const vector<Item>& items, int W) {
    vector<Item> sortedItems = items;
    sort(sortedItems.begin(), sortedItems.end(), compare);
    int totalBenefit = 0;
    for (auto &item : sortedItems) {
        if (item.weight <= W) {
            W -= item.weight;
            totalBenefit += item.benefit;
        }
    }
    return totalBenefit;
}

int main() {
    int sizes[] = {10, 100, 1000, 10000};
    mt19937 rng(100); // Seed value is 100

    cout << fixed << setprecision(3); // Set the precision for floating-point output

    // Greedy
    cout << "[2] Greedy Algorithm" << endl;
    cout << "Number of Items  /  Processing time(ms)  /  Maximum Benefit Value" << endl;

    for (int size : sizes) {
        int W = size * 25;
        vector<Item> items = generateItems(size, rng);

        auto start = high_resolution_clock::now();
        int maxBenefitGreedy = greedyKnapsack(items, W);
        auto end = high_resolution_clock::now();
        duration<double, milli> durationGreedy = end - start;

        cout << size << "  /  " << durationGreedy.count() << "ms" << "  /  " << maxBenefitGreedy << endl;
    }

    // Dynamic Programming
    cout << "[3] Dynamic Programming" << endl;
    cout << "Number of Items  /  Processing time(ms)  /  Maximum Benefit Value" << endl;

    for (int size : sizes) {
        int W = size * 25;
        vector<Item> items = generateItems(size, rng);

        auto start = high_resolution_clock::now();
        int maxBenefitDP = dpKnapsack(items, W);
        auto end = high_resolution_clock::now();
        duration<double, milli> durationDP = end - start;

        cout << size << "  /  " << durationDP.count() << "ms" << "  /  " << maxBenefitDP << endl;
    }

    return 0;
}



// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <chrono>
// #include <queue>
// #include <random>
// #include <iomanip> // Include for setting precision

// using namespace std;
// using namespace chrono;

// struct Item {
//     int benefit;
//     int weight;
// };

// // Function prototypes
// int bruteForceKnapsack(const vector<Item>& items, int W);
// int greedyKnapsack(const vector<Item>& items, int W);
// int dpKnapsack(const vector<Item>& items, int W);
// int bbKnapsack(const vector<Item>& items, int W);

// vector<Item> generateItems(int n, mt19937& rng) {
//     uniform_int_distribution<int> benefitDist(1, 500);
//     uniform_int_distribution<int> weightDist(1, 100);

//     vector<Item> items(n);
//     for (auto& item : items) {
//         item.benefit = benefitDist(rng);
//         item.weight = weightDist(rng);
//     }
//     return items;
// }

// bool compare(const Item& a, const Item& b) {
//     double r1 = (double)a.benefit / a.weight;
//     double r2 = (double)b.benefit / b.weight;
//     return r1 > r2;
// }

// int bruteForceKnapsackHelper(const vector<Item>& items, int W, int i) {
//     if (i == items.size()) return 0;
//     if (items[i].weight > W) return bruteForceKnapsackHelper(items, W, i + 1);
//     return max(bruteForceKnapsackHelper(items, W, i + 1),
//                items[i].benefit + bruteForceKnapsackHelper(items, W - items[i].weight, i + 1));
// }

// int bruteForceKnapsack(const vector<Item>& items, int W) {
//     return bruteForceKnapsackHelper(items, W, 0);
// }

// int greedyKnapsack(const vector<Item>& items, int W) {
//     vector<Item> sortedItems = items;
//     sort(sortedItems.begin(), sortedItems.end(), compare);
//     int totalBenefit = 0;
//     for (auto &item : sortedItems) {
//         if (item.weight <= W) {
//             W -= item.weight;
//             totalBenefit += item.benefit;
//         }
//     }
//     return totalBenefit;
// }

// int dpKnapsack(const vector<Item>& items, int W) {
//     int n = items.size();
//     vector<vector<int>> dp(n+1, vector<int>(W+1, 0));

//     for (int i = 1; i <= n; i++) {
//         for (int w = 0; w <= W; w++) {
//             if (items[i-1].weight <= w) {
//                 dp[i][w] = max(dp[i-1][w], dp[i-1][w-items[i-1].weight] + items[i-1].benefit);
//             } else {
//                 dp[i][w] = dp[i-1][w];
//             }
//         }
//     }
//     return dp[n][W];
// }

// int main() {
//     int sizes[] = {10, 100, 1000, 10000};
//     mt19937 rng(100); // Seed value is 100

//     cout << fixed << setprecision(3); // Set the precision for floating-point output

//     for (int size : sizes) {
//         int W = size * 25;
//         vector<Item> items = generateItems(size, rng);

//         auto start = high_resolution_clock::now();
//         int maxBenefitDP = dpKnapsack(items, W);
//         auto end = high_resolution_clock::now();
//         auto durationDP = duration_cast<duration<double, milli>>(end - start).count();

//         start = high_resolution_clock::now();
//         int maxBenefitGreedy = greedyKnapsack(items, W);
//         end = high_resolution_clock::now();
//         auto durationGreedy = duration_cast<duration<double, milli>>(end - start).count();

//         cout << "Items: " << size
//              << " | DP: " << maxBenefitDP << " / " << durationDP << " ms"
//              << " | Greedy: " << maxBenefitGreedy << " / " << durationGreedy << " ms" << endl;
//     }

//     return 0;
// }


// // #include <iostream>
// // #include <vector>
// // #include <algorithm>
// // #include <chrono>
// // #include <queue>
// // #include <random>

// // using namespace std;
// // using namespace chrono;

// // struct Item {
// //     int benefit;
// //     int weight;
// // };

// // // Function prototypes
// // int bruteForceKnapsack(const vector<Item>& items, int W);
// // int greedyKnapsack(const vector<Item>& items, int W);
// // int dpKnapsack(const vector<Item>& items, int W);
// // int bbKnapsack(const vector<Item>& items, int W);

// // vector<Item> generateItems(int n, mt19937& rng) {
// //     uniform_int_distribution<int> benefitDist(1, 500);
// //     uniform_int_distribution<int> weightDist(1, 100);

// //     vector<Item> items(n);
// //     for (auto& item : items) {
// //         item.benefit = benefitDist(rng);
// //         item.weight = weightDist(rng);
// //     }
// //     return items;
// // }

// // bool compare(const Item& a, const Item& b) {
// //     double r1 = (double)a.benefit / a.weight;
// //     double r2 = (double)b.benefit / b.weight;
// //     return r1 > r2;
// // }

// // int bruteForceKnapsackHelper(const vector<Item>& items, int W, int i) {
// //     if (i == items.size()) return 0;
// //     if (items[i].weight > W) return bruteForceKnapsackHelper(items, W, i + 1);
// //     return max(bruteForceKnapsackHelper(items, W, i + 1),
// //                items[i].benefit + bruteForceKnapsackHelper(items, W - items[i].weight, i + 1));
// // }

// // int bruteForceKnapsack(const vector<Item>& items, int W) {
// //     return bruteForceKnapsackHelper(items, W, 0);
// // }

// // int greedyKnapsack(const vector<Item>& items, int W) {
// //     vector<Item> sortedItems = items;
// //     sort(sortedItems.begin(), sortedItems.end(), compare);
// //     int totalBenefit = 0;
// //     for (auto &item : sortedItems) {
// //         if (item.weight <= W) {
// //             W -= item.weight;
// //             totalBenefit += item.benefit;
// //         }
// //     }
// //     return totalBenefit;
// // }

// // int dpKnapsack(const vector<Item>& items, int W) {
// //     int n = items.size();
// //     vector<vector<int>> dp(n+1, vector<int>(W+1, 0));

// //     for (int i = 1; i <= n; i++) {
// //         for (int w = 0; w <= W; w++) {
// //             if (items[i-1].weight <= w) {
// //                 dp[i][w] = max(dp[i-1][w], dp[i-1][w-items[i-1].weight] + items[i-1].benefit);
// //             } else {
// //                 dp[i][w] = dp[i-1][w];
// //             }
// //         }
// //     }
// //     return dp[n][W];
// // }

// // int main() {
// //     int sizes[] = {10, 100, 1000, 10000};
// //     mt19937 rng(100); // Seed value is 100

// //     for (int size : sizes) {
// //         int W = size * 25;
// //         vector<Item> items = generateItems(size, rng);

// //         auto start = high_resolution_clock::now();
// //         int maxBenefitDP = dpKnapsack(items, W);
// //         auto end = high_resolution_clock::now();
// //         auto durationDP = duration_cast<milliseconds>(end - start).count();

// //         start = high_resolution_clock::now();
// //         int maxBenefitGreedy = greedyKnapsack(items, W);
// //         end = high_resolution_clock::now();
// //         auto durationGreedy = duration_cast<milliseconds>(end - start).count();

// //         cout << "Items: " << size
// //              << " | DP: " << maxBenefitDP << " / " << durationDP << " ms"
// //              << " | Greedy: " << maxBenefitGreedy << " / " << durationGreedy << " ms" << endl;
// //     }

// //     return 0;
// // }
