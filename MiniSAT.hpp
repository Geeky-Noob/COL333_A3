#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

class MiniSAT {
    private:
    int no_vertices;
    int no_edges;
    int k1;
    int k2;
    vector<pair<int, int>> edges;
    vector<vector<int>> graph;
    int clauses=0;
    
    public:
    MiniSAT(string inputfilename, string satFilename);  
    void readInInputFile(string inputfilename);
    void buildGraph();
    void writeCNFtoFile(string filename);
    string constraintOne();
    string constrainttwo();
    string constraintthree();
    string constraintthree2(int k1);
};