#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <queue>
#include <iomanip> // Include for setting precision

using namespace std;
using namespace chrono;

struct Item {
    int benefit;
    int weight;
};

// Branch and Bound knapsack
struct Node {
    int level, profit, weight;
    float bound;

    // Comparison operator
    bool operator<(const Node& other) const {
        return bound < other.bound; // Set lower priority for higher bound
    }
};

// Function prototypes
vector<Item> generateItems(int n, int seed);
vector<Item> generateItems(int n, mt19937& rng);
long long bruteForceKnapsack(const vector<Item>& items, int W);
int dpKnapsack(const vector<Item>& items, int W);
int greedyKnapsack(const vector<Item>& items, int W);
long long bbKnapsack(int W, vector<Item>& items, int n);
bool compare(const Item& a, const Item& b);
float bound(Node u, int n, int W, vector<Item>& items);

// Generate items with random benefits and weights using seed
vector<Item> generateItems(int n, int seed) {
    vector<Item> items(n);
    mt19937 gen(seed);
    uniform_int_distribution<> disValue(1, 500);
    uniform_int_distribution<> disWeight(1, 100);
    
    for (int i = 0; i < n; i++) {
        items[i].benefit = disValue(gen);
        items[i].weight = disWeight(gen);
    }
    return items;
}

// Generate items with random benefits and weights using rng
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

// Brute force knapsack
long long bruteForceKnapsack(const vector<Item>& items, int W) {
    int n = items.size();
    long long maxVal = 0;
    unsigned long long numOfCombinations = 1ULL << n;

    for (unsigned long long i = 0; i < numOfCombinations; i++) {
        int currentWeight = 0, currentValue = 0;
        for (int j = 0; j < n; j++) {
            if (i & (1ULL << j)) {
                currentWeight += items[j].weight;
                currentValue += items[j].benefit;
            }
        }
        if (currentWeight <= W && currentValue > maxVal) {
            maxVal = currentValue;
        }
    }
    return maxVal;
}

// Dynamic programming knapsack
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

// Greedy knapsack
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

bool compare(const Item& a, const Item& b) {
    double r1 = (double)a.benefit / a.weight;
    double r2 = (double)b.benefit / b.weight;
    return r1 > r2;
}

float bound(Node u, int n, int W, vector<Item>& items) {
    if (u.weight >= W)
        return 0;

    float profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;

    while ((j < n) && (totweight + items[j].weight <= W)) {
        totweight += items[j].weight;
        profit_bound += items[j].benefit;
        j++;
    }

    if (j < n)
        profit_bound += (W - totweight) * (double)items[j].benefit / items[j].weight;

    return profit_bound;
}

long long bbKnapsack(int W, vector<Item>& items, int n) {
    sort(items.begin(), items.end(), compare);

    priority_queue<Node> Q; // Use priority queue
    Node u, v;

    u.level = -1;
    u.profit = u.weight = 0;
    Q.push(u);

    long long maxProfit = 0;
    while (!Q.empty()) {
        u = Q.top();
        Q.pop();

        if (u.level == -1)
            v.level = 0;

        if (u.level == n-1)
            continue;

        v.level = u.level + 1;
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].benefit;

        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;

        v.bound = bound(v, n, W, items);
        if (v.bound > maxProfit)
            Q.push(v);

        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, items);
        if (v.bound > maxProfit)
            Q.push(v);
    }

    return maxProfit;
}

int main() {
    int seed = 100;
    int bruteForceSizes[] = {11, 21, 31}; // Sizes for brute force
    int otherSizes[] = {10, 100, 1000, 10000}; // Sizes for other algorithms
    mt19937 rng(seed); // Seed value is 100

    cout << fixed << setprecision(3); // Set the precision for floating-point output

    // Brute Force
    cout << "[1] Brute Force" << endl;
    cout << "Number of Items  /  Processing time(ms)  /  Maximum Benefit Value" << endl;
    for (int n : bruteForceSizes) {
        int W = n * 25;
        auto items = generateItems(n, seed);
        
        auto start = high_resolution_clock::now();
        int maxVal = bruteForceKnapsack(items, W);
        auto stop = high_resolution_clock::now();

        duration<double, milli> duration = stop - start;

        cout << n << "  /  " << duration.count() << "ms" << "  /  " << maxVal << endl;
    }

    // Greedy, Dynamic Programming, Branch and Bound combined output
    cout << "[2] Greedy  /  D.P  /  B.&B." << endl;
    cout << "Number of Items  /  [Processing Time(ms)  /  Maximum Benefit Value]" << endl;

    for (int size : otherSizes) {
        int W = size * 25;
        vector<Item> items = generateItems(size, rng);

        // Greedy
        auto startGreedy = high_resolution_clock::now();
        int maxBenefitGreedy = greedyKnapsack(items, W);
        auto endGreedy = high_resolution_clock::now();
        duration<double, milli> durationGreedy = endGreedy - startGreedy;

        // Dynamic Programming
        auto startDP = high_resolution_clock::now();
        int maxBenefitDP = dpKnapsack(items, W);
        auto endDP = high_resolution_clock::now();
        duration<double, milli> durationDP = endDP - startDP;

        // Branch and Bound
        auto startBB = high_resolution_clock::now();
        int maxBenefitBB = bbKnapsack(W, items, size);
        auto endBB = high_resolution_clock::now();
        duration<double, milli> durationBB = endBB - startBB;

        cout << size << "  /  [" << durationGreedy.count() << "ms  /  " << maxBenefitGreedy << "]  /  [" << durationDP.count() << "ms  /  " << maxBenefitDP << "]  /  [" << durationBB.count() << "ms  /  " << maxBenefitBB << "]" << endl;
    }

    return 0;
}
