#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "MiniSAT.hpp"
using namespace std;

MiniSAT::MiniSAT(string inputfilename, string satFilename){
    
    readInInputFile(inputfilename);
    // cout << "Vertices: " << no_vertices << ", Edges: " << no_edges << endl;
    // cout << "k1: " << k1 << ", k2: " << k2 << endl;
    
    // for (const auto& edge : edges) {
    //     cout << edge.first << " " << edge.second << endl;
    // }
    
    buildGraph();
    
    // for (int i = 1; i < graph.size(); ++i) {
    //     cout << "Node " << i << " is connected to: ";
    //     for (const int& neighbor : graph[i]) {
    //         cout << neighbor << " ";
    //     }
    //     cout << endl;
    // }
    
    
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
    stringstream s1,s2;
    for (int i = 1; i <= no_vertices; ++i) {
        cnf << "-" << i << " -" << (i + no_vertices) << " 0\n";
        if (k1 > 0) {
            s1 << i <<" ";
        }
        if (k2 > 0) {
            s2 << (i+no_vertices) <<" ";
        }   
    }
    s1 << "0\n";
    s2 << "0\n";
    clauses = no_vertices+2;
    return cnf.str()+s1.str()+s2.str();
}

string MiniSAT::constrainttwo() {
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
                    cnf << "-" << (i + no_vertices) << " -" <<  (j + no_vertices) << " 0\n";
                    clauses = clauses + 2;
                }
            }
        }
    }
    return cnf.str();
}

// using sequential circuit encoding
string MiniSAT::constraintthree() {
    stringstream cnf;

    // for G1: x1,x2,..,xn
    if (no_vertices-k1>0){
        cnf << 1 << " " << 2*no_vertices+1 << " 0\n";
        clauses++;
        for (int j = 2; j<=(no_vertices-k1); ++j) {
            cnf << "-" << 2*no_vertices+j << " 0\n";
            clauses++;
        }
        for (int i = 2; i < no_vertices; ++i) {
            cnf << i << " " << (2*no_vertices+(no_vertices-k1)*(i-1)+1) << " 0\n";
            cnf << "-" << (2*no_vertices+(no_vertices-k1)*(i-2)+1) << " " << (2*no_vertices+(no_vertices-k1)*(i-1)+1) << " 0\n";
            clauses = clauses+2;
            for (int j = 2; j<=(no_vertices-k1); ++j) {
                cnf << i << " -" << (2*no_vertices+(no_vertices-k1)*(i-2)+j-1) << " " << (2*no_vertices+(no_vertices-k1)*(i-1)+j) << " 0\n";
                cnf << "-" << (2*no_vertices+(no_vertices-k1)*(i-2)+j) << " " << (2*no_vertices+(no_vertices-k1)*(i-1)+j) << " 0\n";
                clauses = clauses+2;
            }
            cnf << i << " -" << (2*no_vertices+(no_vertices-k1)*(i-2)+(no_vertices-k1)) << " 0\n";
            clauses++;
        }
        cnf << no_vertices << " -" << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)) << " 0\n";
        clauses++;
    } else if (no_vertices-k1<=0) {
        for (int i=1; i<=no_vertices; ++i){
            cnf << "-" << i << " 0\n";
            clauses++;
        }
    }
    
    // for G2: y1,y2,..,yn
    if (no_vertices-k1>0 && no_vertices-k2>0){
        cnf << (1+no_vertices) << " " << (2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+1 << " 0\n";
        clauses++;
        for (int j = 2; j<=(no_vertices-k2); ++j) {
            cnf << "-" << (2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+j << " 0\n";
            clauses++;
        }
        for (int i = 2; i < no_vertices; ++i) {
            cnf << (i+no_vertices) << " " << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-1)+1) << " 0\n";
            cnf << "-" << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-2)+1) << " " << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-1)+1) << " 0\n";
            clauses = clauses+2;
            for (int j = 2; j<=(no_vertices-k2); ++j) {
                cnf << (i+no_vertices) << " -" << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-2)+j-1) << " " << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-1)+j) << " 0\n";
                cnf << "-" << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-2)+j) << " " << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-1)+j) << " 0\n";
                clauses = clauses+2;
            }
            cnf << (i+no_vertices) << " -" << ((2+(no_vertices-k1))*no_vertices-(no_vertices-k1)+(no_vertices-k2)*(i-2)+(no_vertices-k2)) << " 0\n";
            clauses++;
        }
        cnf << 2*no_vertices << " -" << ((2+(no_vertices-k1)+(no_vertices-k2))*no_vertices-(no_vertices-k2)-(no_vertices-k1)) << " 0\n";
        clauses++;
    } else if (no_vertices-k2<=0){
        for (int i=1; i<=no_vertices; ++i){
            cnf << "-" << (i+no_vertices) << " 0\n";
            clauses++;
        }
    }

    return cnf.str();
}

void MiniSAT::writeCNFtoFile(string filename) {
    string cnf1 = constraintOne();
    string cnf2 = constrainttwo();
    string cnf3 = constraintthree();
    string cnfFormula = cnf1 + cnf2 + cnf3;
    // cout << "CNF:\n" << cnfFormula << endl;
    ofstream satInputFile(filename);
    if (!satInputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    satInputFile << "c Here is a comment." << endl;
    // satInputFile << "p cnf " << no_vertices*((2*no_vertices-k1)+(no_vertices-k2)+2) << " " << (2*no_vertices) + (2*no_vertices*(no_vertices-k1)+no_vertices-3*(no_vertices-k1)-1) + (2*no_vertices*(no_vertices-k2)+no_vertices-3*(no_vertices-k2)-1) << endl;

    int variables = no_vertices*(2+2*no_vertices-k1-k2) - 2*no_vertices + k1 + k2;
    satInputFile << "p cnf " << variables << " " << clauses << endl;
    satInputFile << cnfFormula;
    satInputFile.close();
}
