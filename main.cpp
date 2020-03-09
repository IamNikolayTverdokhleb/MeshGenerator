#include <iostream>
#include "meshGenerator.h"
#include "solverObject.h"
int main() {
    auto *obj  = new solverObject();
    obj->setSystem();
    obj->assembleSystem();
    obj->solveSystem();
    obj->visualizeSolution();
    /*auto *obj  = new meshGenerator("../inputFile.txt");
    obj -> readFile();
    obj -> fileOutput();
    obj -> twoDimensionalMesh();*/
    return 0;
}