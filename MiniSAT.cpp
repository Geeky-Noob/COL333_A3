
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "MiniSAT.hpp"
using namespace std;

MiniSAT::MiniSAT(string inputfilename, string satFilename){
    
    readInInputFile(inputfilename);
    cout << "Vertices: " << no_vertices << ", Edges: " << no_edges << endl;
    cout << "k1: " << k1 << ", k2: " << k2 << endl;
    
    for (const auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    
    buildGraph();
    
    for (int i = 1; i < graph.size(); ++i) {
        cout << "Node " << i << " is connected to: ";
        for (const int& neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
    
    
    writeCNFtoFile(satFilename);
}

void MiniSAT::readInInputFile(string inputfilename) {
    
    fstream ipfile;
    ipfile.open(inputfilename, ios::in);
    if (!ipfile) {
        cout << "No such file\n";
        exit( 0 );
    } else {
        ipfile >> no_vertices;
        ipfile >> no_edges;
        ipfile >> k1;
        ipfile >> k2;
        
        int u,v;
        
        for (int i=1; i <= no_edges; i++) {
            ipfile >> u >> v;
            edges.push_back({u, v});
        }
        ipfile.close();
    }
}

void MiniSAT::buildGraph() {  // adjacency list
    vector<vector<int>> tempGraph(no_vertices+1);
    for (const auto &edge : edges) {
        tempGraph[edge.first].push_back(edge.second);
        tempGraph[edge.second].push_back(edge.first);
    }
    graph = tempGraph;
}

string MiniSAT::constraintOne() {
    stringstream cnf;
    for (int i = 1; i <= no_vertices; ++i) {
        cnf << "-" << i << " -" << (i + no_vertices) << " 0\n";
    }
    // for (int i = 1; i <= no_vertices; ++i) {
    //     cnf << i << " " << (i + no_vertices) << " 0\n";
    // }
    return cnf.str();
}

string MiniSAT::constrainttwo() {
    stringstream cnf;
    for (int i = 1; i <= no_vertices; ++i) {
        for (int neighbour : graph[i]){
            cnf << "-" << i << " " <<  neighbour << " 0\n";
            cnf << "-" << (i + no_vertices) << " " <<  (neighbour + no_vertices) << " 0\n";
        }
    }
    return cnf.str();
}

void MiniSAT::writeCNFtoFile(string filename) {
    string cnf1 = constraintOne();
    string cnf2 = constrainttwo();
    string cnfFormula = cnf1 + cnf2;
    cout << "CNF:\n" << cnfFormula << endl;
    ofstream satInputFile(filename);
    if (!satInputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    satInputFile << "c Here is a comment." << endl;
    satInputFile << "p cnf 14 7" << endl;
    satInputFile << cnfFormula;
    satInputFile.close();
}

