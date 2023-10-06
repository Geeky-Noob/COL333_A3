/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "MiniSAT.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <argument1> <argument2>" << endl;
        return 1;
    }
    string inputfilename = argv[1];
    string satfilename = argv[2];
    // string inputfilename ("test.graph");
    // string satfilename ("test.satinput");
    MiniSAT *allotment = new MiniSAT(inputfilename, satfilename);
    return 0;
}


