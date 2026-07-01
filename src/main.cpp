#include <vector>
#include <cstdio>
#include <limits>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

vector<vector<float>> convertToMatrix(const char * name_file, int *size) {
    ifstream f(name_file);

    vector<vector<float>> matrix;

    if (!f.is_open()) {
        cerr << "Error: Could not open file " << name_file << "\n";
        *size = 0;
        return matrix;
    }

    string line;
    int nodes = 0, cols = 0, edges = 0;

    // Skip comment lines (lines starting with '%')
    while (getline(f, line)) {
        if (line.empty() || line[0] == '%') {
            continue;
        }
        // Read the size line
        stringstream ss(line);
        if (ss >> nodes >> cols >> edges) {
            break;
        }
    }

    *size = nodes;
    matrix.assign(nodes, vector<float>(nodes, numeric_limits<float>::infinity()));

    // Read edges

    int src, dst;
    float weight;
    int i = 0;
    while (i < edges && f >> src >> dst >> weight) {
        matrix[src - 1][dst - 1] =  weight;
        i++;
    }

    if (i < edges) {
        cerr << "Warning: File has " << edges << "edges, but only " << i << "were loaded";
    }

    return matrix;
}

vector<tuple<int, int, float>>  convertToList(const string& name_file, int *size) {

    ifstream f(name_file);
    vector<tuple<int, int, float>> edge_list;

    if (!f.is_open()) {
        cerr << "Error: Could not open file " << name_file << "\n";
        *size = 0;
        return edge_list;
    }

    string line;
    int nodes = 0, cols = 0, edges = 0;

    // Skip comment lines (lines starting with '%')
    while (getline(f, line)) {
        if (line.empty() || line[0] == '%') {
            continue;
        }
        // Read the size line
        stringstream ss(line);
        if (ss >> nodes >> cols >> edges) {
            break;
        }
    }

    *size = nodes;
    edge_list.reserve(edges);

    int src, dst;
    float weight;
    int i = 0;
    while (i < edges && f >> src >> dst >> weight) {
        edge_list.emplace_back(src - 1, dst -1, weight);
        i++;
    }

    if (i < edges) {
        cerr << "Warning: File has " << edges << "edges, but only " << i << "were loaded";
    }
    return edge_list;
}

vector<vector<float>> floydWarshall(const vector<vector<float>>& adjacency, int size){
    vector<vector<float>> path = adjacency;

    // Necesario para el alg, pero podemos pasarlo a la lectura de archivos
    for(int i = 0; i < size; i++) {
        path[i][i] = 0;
    }
    
    for(int k = 0; k < size; k++){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                float dt = path[i][k] + path[k][j];
                if(path[i][j] > dt)
                    path[i][j] = dt;
            }
        }
    }
    
    return path;
}

vector<float> bellmanFord(const vector<tuple<int, int, float>>& edges, int V, int src) {
    
	vector<float> dist(V, numeric_limits<float>::infinity());
    dist[src] = 0;
	for (int i = 0; i < V; i++) {
		for (const auto& [u, v, weight] : edges) {
			if (dist[u] != numeric_limits<float>::infinity() && dist[u] + weight < dist[v]) {
			    
                // If this is the Vth relaxation, then there is
                // a negative cycle
                if(i == V - 1)
                    return {-1};
               
                // Update shortest distance to node v
                dist[v] = dist[u] + weight;
            }
		}
	}

    return dist;
}

int main() {
    const char* name_file = "graph.mtx";
    int size = 0;

    vector<vector<float>> matrix = convertToMatrix(name_file, &size);
    vector<vector<float>> path = floydWarshall(matrix, size);

    cout << "done" << endl;
    return 0;
}
