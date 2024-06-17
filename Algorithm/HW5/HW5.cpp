// 강의 slide chapter 22
// blog: **https://sarah950716.tistory.com/12#footnote_link_12_1**
// blog: **https://suyeon96.tistory.com/32**

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

// 인접 행렬을 읽어들이는 함수
void readMatrix(const string& file_path, vector<char>& headers, vector<vector<int>>& matrix) {
    ifstream file(file_path);
    string line;

    if (file.is_open()) {
        getline(file, line); // 첫 번째 줄 읽기
        stringstream ss(line);
        char header;
        while (ss >> header) {
            headers.push_back(header);
        }

        while (getline(file, line)) {
            stringstream ss(line);
            char row_header;
            ss >> row_header;
            vector<int> row;
            int value;
            while (ss >> value) {
                row.push_back(value);
            }
            matrix.push_back(row);
        }
    } else {
        cerr << "Failed to open file: " << file_path << endl;
    }
}

// 인접 행렬을 인접 리스트로 변환하는 함수
unordered_map<char, vector<char>> matrixToAdjList(const vector<char>& headers, const vector<vector<int>>& matrix) {
    unordered_map<char, vector<char>> adj_list;
    for (int i = 0; i < headers.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 1) {
                adj_list[headers[i]].push_back(headers[j]);
            }
        }
    }
    return adj_list;
}

// 인접 리스트의 전치 그래프를 생성하는 함수
unordered_map<char, vector<char>> transposeAdjList(const unordered_map<char, vector<char>>& adj_list) {
    unordered_map<char, vector<char>> transposed_list;
    for (const auto& pair : adj_list) {
        for (char neighbor : pair.second) {
            transposed_list[neighbor].push_back(pair.first);
        }
    }
    return transposed_list;
}

// 인접 리스트를 출력하는 함수
void printAdjList(const unordered_map<char, vector<char>>& adj_list, const string& title) {
    cout << title << endl;
    map<char, vector<char>> sorted_adj_list(adj_list.begin(), adj_list.end());
    for (auto& pair : sorted_adj_list) {
        sort(pair.second.begin(), pair.second.end()); // 각 리스트를 정렬
        cout << pair.first << ": ";
        for (char neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

int main() {
    string file_path = "hw5_data.txt"; 
    vector<char> headers;
    vector<vector<int>> matrix;

    readMatrix(file_path, headers, matrix);

    auto adj_list = matrixToAdjList(headers, matrix);
    auto transposed_list = transposeAdjList(adj_list);

    printAdjList(adj_list, "Array of adjacency list of above graph");
    printAdjList(transposed_list, "Array of adjacency list of transpose graph");

    return 0;
}

