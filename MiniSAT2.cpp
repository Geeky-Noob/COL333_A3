
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "MiniSAT2.hpp"
using namespace std;

MiniSAT2::MiniSAT2(string inputfilename, string satFilename, int k){
    
    readInInputFile(inputfilename);
    cout << "Vertices: " << no_vertices << ", Edges: " << no_edges << endl;
    
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
    
    writeCNFtoFile(satFilename, k);
}

void MiniSAT2::readInInputFile(string inputfilename) {
    
    fstream ipfile;
    ipfile.open(inputfilename, ios::in);
    if (!ipfile) {
        cout << "No such file\n";
        exit( 0 );
    } else {
        ipfile >> no_vertices;
        ipfile >> no_edges;
        
        int u,v;
        
        for (int i=1; i <= no_edges; i++) {
            ipfile >> u >> v;
            edges.push_back({u, v});
        }
        ipfile.close();
    }
}

void MiniSAT2::buildGraph() {  // adjacency list
    vector<vector<int>> tempGraph(no_vertices+1);
    for (const auto &edge : edges) {
        tempGraph[edge.first].push_back(edge.second);
        tempGraph[edge.second].push_back(edge.first);
    }
    graph = tempGraph;
}

string MiniSAT2::constraintOne() {
    stringstream cnf;
    for (int i=1; i<=no_vertices; i++) {
        cnf << i << " ";
    }
    cnf << "0\n";

    return cnf.str();
}

string MiniSAT2::constrainttwo(int k) {
    stringstream cnf;
    for (int i = 1; i <= no_vertices; ++i) {
        for (int j = 1; j <= no_vertices; ++j) {

            if (i != j) {
                bool connected = false;
                for (int neighbour : graph[i]){
                    if (neighbour == j){
                        connected = true;
                        break;
                    } 
                }

                if (!connected) {
            
                    cnf << "-" << i << " -" <<  j << " 0\n";
                }
            }
        }
    }
    return cnf.str();
}

// using sequential circuit encoding
string MiniSAT2::constraintthree2(int k) {
    stringstream cnf;

    // for G1: x1,x2,..,xn
    if (no_vertices-k>0) {
        cnf << 1 << " " << no_vertices+1 << " 0\n";
        for (int j = 2; j<=(no_vertices-k); ++j) {
            cnf << "-" << no_vertices+j << " 0\n";
        }
        for (int i = 2; i < no_vertices; ++i) {
            cnf << i << " " << (no_vertices+(no_vertices-k)*(i-1)+1) << " 0\n";
            cnf << "-" << (no_vertices+(no_vertices-k)*(i-2)+1) << " " << (no_vertices+(no_vertices-k)*(i-1)+1) << " 0\n";
            for (int j = 2; j<=(no_vertices-k); ++j) {
                cnf << i << " -" << (no_vertices+(no_vertices-k)*(i-2)+j-1) << " " << (no_vertices+(no_vertices-k)*(i-1)+j) << " 0\n";
                cnf << "-" << (no_vertices+(no_vertices-k)*(i-2)+j) << " " << (no_vertices+(no_vertices-k)*(i-1)+j) << " 0\n";
            }
            cnf << i << " -" << (no_vertices+(no_vertices-k)*(i-2)+(no_vertices-k)) << " 0\n";
        }
        cnf << no_vertices << " -" << ((1+(no_vertices-k))*no_vertices-(no_vertices-k)) << " 0\n";
    } else if (no_vertices-k==0) {
        for (int i=1; i<=no_vertices; ++i){
            cnf << "-" << i << " 0\n";
        }
    }


    return cnf.str();
}

void MiniSAT2::writeCNFtoFile(string filename, int k) {
    string cnf1 = constraintOne();
    string cnf2 = constrainttwo(k);
    string cnf3 = constraintthree2(k);
    string cnfFormula = cnf1 + cnf2 + cnf3;
    // cout << "CNF:\n" << cnfFormula << endl;
    ofstream satInputFile(filename);
    if (!satInputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    satInputFile << "c Here is a comment." << endl;
    satInputFile << "p cnf 14 14\n";
    // satInputFile << "p cnf " << no_vertices*((2*no_vertices-k1)+(no_vertices-k2)+2) << " " << (2*no_vertices) + (2*no_vertices*(no_vertices-k1)+no_vertices-3*(no_vertices-k1)-1) + (2*no_vertices*(no_vertices-k2)+no_vertices-3*(no_vertices-k2)-1) << endl;
    satInputFile << cnfFormula;
    satInputFile.close();
}
