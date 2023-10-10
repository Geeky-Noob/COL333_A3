#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

class MiniSAT2 {
    private:
    int no_vertices;
    int no_edges;
    vector<pair<int, int>> edges;
    vector<vector<int>> graph;
    int clauses=0;
    
    public:
    MiniSAT2(string inputfilename, string satFilename, int k);  
    void readInInputFile(string inputfilename);
    void buildGraph();
    void writeCNFtoFile(string filename, int k);
    string constraintOne();
    string constrainttwo(int k);
    string constraintthree2(int k1);
};