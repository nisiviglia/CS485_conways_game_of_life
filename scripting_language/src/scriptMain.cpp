/*****************
Parser Main
*****************/

#include <iostream>
#include <vector>
#include <fstream>
#include "../include/ParseNode.h"

using namespace std;

// int currentLine = 0;

int globalErrorCount = 0;
string world_name = "dummy";
vector<pair<int,int>> live_cells;

int
main(int argc, char *argv[])
{
    ifstream in;
    in.open(argv[1], ios::in);

    if (!in.is_open()){
        cout << "Error, could not open file!" << endl;
        return 1;
    }

    

    ParseNode *script = Script(in);  
    // ParseNode *program;
    // program = new StatementList(new SetStatement("customid", new Iconst(10)), 0);

    if( script == 0 || globalErrorCount > 0 ) {
        cout << "Program failed!" << endl;
        return 1;
    }

    std::vector<string> loc;

    // PostorderTraversal
    script->Action(loc);

    return 0;
}