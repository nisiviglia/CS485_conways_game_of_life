#include "../include/ObjDispatcher.hpp"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    cout << "creating class" << endl;
    ol::ObjDispatcher dispatch; 

    cout << "create world \"testWorld\"" << endl;
    int rc = dispatch.Single(ol::INSERT, ol::WORLD, "", "testWorld", 0);

    if(rc)
        std::cout << "error: " << dispatch.GetErrorLog() << std::endl;

    return 0;
}
