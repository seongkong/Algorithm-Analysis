#include <fstream>
#include <iostream>

using namespace std;

const int num_nodes = 5;

int main() {
  head_node my_graph[num_nodes];
  int distance[num_nodes][num_nodes];

  Create_array(my_graph);

  for(int i = 0; i < num_nodes; i++) {
    cout << my_graph[i].vertex << ", ";
    cout << my_graph[i].link << endl;
  }
  cout << endl;

  Read_from_file(distance);
}

void Read_from_file(int distance[][num_nodes]) {
  ifstream inputFile ("data.txt");
  for(int i = 0; i < num_nodes; i++) {
    for(int j = 0; j < num_nodes; j++) {
      inputFile >> distance[i][j];
      cout << distance[i][j] << '\t';
    }
    cout << endl;
  }
  cout << endl;
}

typedef struct h_node {
  int vertex;
  struct g_node *link;
} head_node;

typedef struct g_node {
  int city;
  int distance;
  struct g_node *link;
} list_node;

void Create_array(head_node my_graph[]) {
  for(int i = 0; i < num_nodes; i++) {
    my_graph[i].vertex = i;
    my_graph[i].link = NULL;
  }
}