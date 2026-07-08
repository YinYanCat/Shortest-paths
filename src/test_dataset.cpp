#include "paths.cpp"
#include <vector>
#include <tuple>
#include <fstream>
#include <chrono>
#include <string>
#include <iostream>

using namespace std;

int main(){
    int size;
    string dataset = "bio-SC-TS.edges";
    vector<vector<float>> matrix = edgesToMatrix(dataset.c_str(), &size);
    
    auto begin_time = std::chrono::high_resolution_clock::now();
    vector<vector<float>> result = floydWarshall(matrix, size);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed_time = end_time - begin_time;

    vector<tuple<int, int, float>> edges = toGraphEdges(result, size);

    ofstream result_edges(dataset + "_all_shortest_paths_result.csv");

    if (!result_edges.is_open()) {
        cerr << "Error: Could not open file " << dataset + "_all_shortest_paths_result.csv" << "\n";
        return 1;
    }

    for (const auto& edge : edges) {
        int u = get<0>(edge);
        int v = get<1>(edge);
        float weight = get<2>(edge);
        result_edges << u + 1 << " " << v + 1 << " " << weight << endl;
    }

    result_edges.close();

    ofstream time_f(dataset + "_calculation_time.csv");

    if (!time_f.is_open()) {
        cerr << "Error: Could not open file " << dataset + "_all_shortest_paths_result.csv" << "\n";
        return 1;
    }
    time_f << elapsed_time.count();
    time_f.close();
    return 0;
}