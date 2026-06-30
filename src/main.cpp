#include <vector>
#include <cstdio>
#include <limits>

using namespace std;

std::vector<std::vector<float>> convertToMatrix(const char * name_file, int *size) {

    FILE* f = std::fopen(name_file, "r");

    // Skip the header line (%%MatrixMarket ...)
    char line[256];
    std::fgets(line, sizeof(line), f);

    // Skip comment lines (lines starting with '%')
    while (std::fgets(line, sizeof(line), f)) {
        if (line[0] != '%') break;
    }

    // Read the size line
    long long nodes, cols, edges;
    std::sscanf(line, "%lld %lld %lld", &nodes, &cols, &edges);
    *size = nodes;
    std::vector<std::vector<float>> matrix(nodes, std::vector<float>(nodes, std::numeric_limits<float>::infinity()));

    // Read edges
    for (long long i = 0; i < edges; i++) {
        int src, dst;
        float weight;
        
        if (fscanf(f, "%d %d %f", &src, &dst, &weight) != 3) {
            printf("Error reading entry %d\n", i);
            break;
        }
        int u = src - 1;
        int v = dst - 1;
        matrix[u][v] = weight;
    }
    fclose(f);
    return matrix;
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


int main() {
    const char* name_file = "graph.mtx";
    int size = 0;

    std::vector<std::vector<float>> matrix = convertToMatrix(name_file, &size);
    vector<vector<float>> path = floydWarshall(matrix, size);

    return 0;
}
