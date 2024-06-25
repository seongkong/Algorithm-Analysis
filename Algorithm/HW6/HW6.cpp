// 강의 Slide chapter 25
// Blog: https://charles098.tistory.com/11
// Blog: https://yabmoons.tistory.com/364

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <iomanip>

using namespace std;

const int INF = numeric_limits<int>::max();
const int WIDTH = 8;  // 각 열의 너비를 설정

void readGraph(const string& filename, vector<vector<int>>& graph, vector<string>& cities) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string value;
        vector<int> row;
        bool isFirstValue = true;

        while (iss >> value) {
            if (isFirstValue) {
                cities.push_back(value); // 첫 번째 값은 도시 이름
                isFirstValue = false;
            } else {
                try {
                    if (value == "INF") {
                        row.push_back(INF);
                    } else {
                        row.push_back(stoi(value));
                    }
                } catch (const invalid_argument& e) {
                    cerr << "Invalid data found in file: " << value << endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        if (!row.empty()) {
            graph.push_back(row);
        }
    }
}

void printGraph(const vector<vector<int>>& graph, const vector<string>& cities) {
    cout << setw(WIDTH) << left << "";
    for (const auto& city : cities) {
        cout << setw(WIDTH) << left << city;
    }
    cout << "\n";

    for (size_t i = 0; i < graph.size(); ++i) {
        cout << setw(WIDTH) << left << cities[i];
        for (const auto& val : graph[i]) {
            if (val == INF) {
                cout << setw(WIDTH) << left << "INF";
            } else {
                cout << setw(WIDTH) << left << val;
            }
        }
        cout << "\n";
    }
}

void dijkstra(const vector<vector<int>>& graph, int src, vector<int>& dist) {
    int V = graph.size();
    vector<bool> sptSet(V, false);
    
    dist.assign(V, INF);
    dist[src] = 0;

    for (int count = 0; count < V - 1; ++count) {
        int u = -1;
        for (int i = 0; i < V; ++i) {
            if (!sptSet[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        sptSet[u] = true;
        
        for (int v = 0; v < V; ++v) {
            if (!sptSet[v] && graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

void floydWarshall(vector<vector<int>>& dist) {
    int V = dist.size();
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

int main() {
    vector<vector<int>> graph;
    vector<string> cities;
    readGraph("hw6.data", graph, cities);

    cout << "Input Graph:\n";
    printGraph(graph, cities);
    cout << "\n";

    int V = graph.size();
    vector<vector<int>> dijkstraResult(V, vector<int>(V, INF));

    for (int i = 0; i < V; ++i) {
        dijkstra(graph, i, dijkstraResult[i]);
    }

    cout << "Shortest distances using Dijkstra's algorithm:\n";
    printGraph(dijkstraResult, cities);
    cout << "\n";

    vector<vector<int>> floydResult = graph;
    floydWarshall(floydResult);

    cout << "Shortest distances using Floyd's algorithm:\n";
    printGraph(floydResult, cities);

    return 0;
}
