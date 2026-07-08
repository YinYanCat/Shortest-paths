#include "paths.cpp"
#include <vector>
#include <limits>
#include <tuple>

using namespace std;

int main(){

    vector<vector<float>> matrix(6, vector<float>(6, numeric_limits<float>::infinity()));

    for(int i = 0; i < 6; i++){
        matrix[i][i] = 0;
    }

    matrix[0][1] = 12;
    matrix[0][4] = 13;
    matrix[1][2] = 14;
    matrix[1][4] = 15;
    matrix[2][3] = 16;
    matrix[3][4] = 17;
    matrix[3][5] = 18;

    cout << "Original Adjacency Matrix:" << endl;

    int i = 0;

    cout << "\t";
    for(int j = 0; j < 6; j++){
        cout << "[" << j << "]\t";
    }
    cout << endl;
    for(auto vec : matrix){
        cout << "[" << i++ <<"]\t";
        for(float val : vec){
            cout << val << "\t";
        }
        cout << endl;
    }

    cout << endl << "All shortest path (Floyd-Warshall):" << endl;
    
    i = 0;
    cout << "\t";
    for(int j = 0; j < 6; j++){
        cout << "[" << j << "]\t";
    }
    cout << endl;

    for(auto vec : floydWarshall(matrix, 6)){
        cout << "[" << i++ <<"]\t";
        for(float val : vec){
            cout << val << "\t";
        }
        cout << endl;
    }

    // copypaste
    cout << endl << "All shortest path (Base):" << endl;
    vector<tuple<int,int,float>> edges = toGraphEdges(matrix, 6);
    i = 0;
    cout << "\t";
    for(int j = 0; j < 6; j++){
        cout << "[" << j << "]\t";
    }
    cout << endl;

    for(auto vec : baseAlgorithm(edges, 6)){
        cout << "[" << i++ <<"]\t";
        for(float val : vec){
            cout << val << "\t";
        }
        cout << endl;
    }

    return 0;
}