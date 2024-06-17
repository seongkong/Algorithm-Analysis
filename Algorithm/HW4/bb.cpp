#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>
#include <iomanip> // 소수점 출력을 위한 헤더

using namespace std;
using namespace chrono;

struct Item {
    int value;
    int weight;
};

struct Node {
    int level, profit, weight;
    float bound;

    // 비교 연산자 정의
    bool operator<(const Node& other) const {
        return bound < other.bound; // 최대 힙을 위해 바운드 값이 더 클 때 더 낮은 우선 순위를 갖도록 설정
    }
};

bool cmp(Item a, Item b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
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
        profit_bound += items[j].value;
        j++;
    }

    if (j < n)
        profit_bound += (W - totweight) * (double)items[j].value / items[j].weight;

    return profit_bound;
}

long long knapsack(int W, vector<Item>& items, int n) {
    sort(items.begin(), items.end(), cmp);

    priority_queue<Node> Q; // 우선순위 큐 사용
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
        v.profit = u.profit + items[v.level].value;

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

vector<Item> generateItems(int n) {
    vector<Item> items;
    mt19937 gen(100); // Standard mersenne_twister_engine seeded with 100
    uniform_int_distribution<> dis_value(1, 500);
    uniform_int_distribution<> dis_weight(1, 100);

    for (int i = 0; i < n; i++) {
        int value = dis_value(gen);
        int weight = dis_weight(gen);
        items.push_back({value, weight});
    }

    return items;
}

int main() {
    vector<int> itemSizes = {10, 100, 1000, 10000};

    cout << fixed << setprecision(3); // Set the precision for floating-point output
    cout << "[4] Branch and Bound" << endl;
    cout << "Number of Items  /  Processing time(ms)  /  Maximum Benefit Value" << endl;

    for (int n : itemSizes) {
        int W = n * 25;
        auto items = generateItems(n);

        auto start = high_resolution_clock::now();
        int maxProfit = knapsack(W, items, n);
        auto stop = high_resolution_clock::now();

        duration<double, milli> duration = stop - start; // Explicitly defining the type

        cout << n << "  /  " << duration.count() << "ms" << "  /  " << maxProfit << endl;
    }

    return 0;
}