#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "MiniSAT2.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <argument1> <argument2> <argument3>" << endl;
        return 1;
    }
    string inputfilename = argv[1];
    string satfilename = argv[2];
    string k1 = argv[3];
    int k = stoi(k1);
    // string inputfilename ("test.graph");
    // string satfilename ("test.satinput");
    MiniSAT2 *allotment = new MiniSAT2(inputfilename, satfilename, k);
    return 0;
}