#include <iostream>
#include "meshGenerator.h"
int main() {
    std::string fileName = "/Users/kola/Desktop/РПК/MeshGenerator/inputFile.txt";
    auto *obj = new meshGenerator{fileName};
    obj -> readFile();
    obj -> lineMesh();
    obj -> fileOutput();
    return 0;
}