#include <vector>
#include <tuple>
#include <cstdio>
#include <limits>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <random>

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

vector<tuple<int, int, float>> toGraphEdges(vector<vector<float>> adjacency, int V){
    vector<tuple<int, int, float>> edges;
    edges.reserve(V*(V-1));

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                continue;
            }
            if (adjacency[i][j] != numeric_limits<float>::infinity()) {
                edges.push_back({i, j, adjacency[i][j]});
            }
        }
    }
    return edges;
}
float densityEdgeProbability(int V, float density) {
    // Probability (p) of the existence of an edge given a expected density
    if (V <= 2) return 0.0f;
    float p = (density * V - 2.0f) / (V - 2.0f);
    return (p < 0.0f) ? 0.0f : p;
}

vector<vector<float>> connectedGraphAdjacency(int V, float edge_p){
    vector<vector<float>> adjacecy(V, vector<float>(V, numeric_limits<float>::infinity()));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis_p(0.0f, 1.0f);
    std::uniform_real_distribution<float> dis_weight(1e-3, 1e3);
    
    for (int i = 0; i < V; i++) {
        adjacecy[i][i] = 0;
    }

    // Strongly connected (bidirectional path)
    for (int i = 0; i < V-1; i++) {
        adjacecy[i][i+1] = dis_weight(gen);
        adjacecy[i+1][i] = dis_weight(gen);
    }

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
               continue;
            }
            if(adjacecy[i][j] == numeric_limits<float>::infinity()) {
                if (dis_p(gen) < edge_p){
                    adjacecy[i][j] = dis_weight(gen);
                }
            }
        }
    }
    return adjacecy;
}

vector<vector<float>> concatGraphAdjacency(const vector<vector<float>> &g1, const vector<vector<float>> &g2, int v1, int v2) {
    vector<vector<float>> g;
    g.reserve(v1 + v2);
    vector<float> r_inf(v2, numeric_limits<float>::infinity());
    vector<float> l_inf(v1, numeric_limits<float>::infinity());
    for (int i = 0; i < v1; i++) {
        vector<float> row = g1[i];
        row.insert(row.end(), r_inf.begin(), r_inf.end());
        g.push_back(row);
    }
    for (int i = 0; i < v2; i++) {
        vector<float> row = l_inf;
        row.insert(row.end(), g2[i].begin(), g2[i].end());
        g.push_back(row);
    }
    return g;
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
		for (const auto& edge : edges) {
            int u = get<0>(edge);
            int v = get<1>(edge);
            float weight = get<2>(edge);
			if (dist[u] != numeric_limits<float>::infinity() && dist[u] + weight < dist[v]) {
			    
                // If this is the Vth relaxation, then there is
                // a negative cycle
                if(i == V - 1) 
                    throw runtime_error("The given Graph contains a negative cycle.");
               
                // Update shortest distance to node v
                dist[v] = dist[u] + weight;
            }
		}
	}

    return dist;
}

vector<vector<float>> baseAlgorithm(const vector<tuple<int, int, float>>& edges, int V) {
    vector<vector<float>> path(V, vector<float>(V, numeric_limits<float>::infinity()));

    for (int i = 0; i < V; i++) {
        path[i] = bellmanFord(edges, V, i);
    }
    return path;
}
