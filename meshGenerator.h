#ifndef MESHGENERATOR_MESHGENERATOR_H
#define MESHGENERATOR_MESHGENERATOR_H
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>

class meshGenerator {
public:
    explicit meshGenerator(std::string inputFileName): fileName(std::move(inputFileName)){
    std::cout << "Mesh generation started." << std::endl;
    }
    void readFile();
    void lineMesh();
    void fileOutput();
    inline unsigned int getSizeOfMeshPointsArray();
    inline std::vector<double> getMeshPointsArray();
    inline int getNumberOfElements();
    std::vector<double> meshPointsArray;
private:
    std::string fileName;
    uint64_t numberOfDimensions{0};
    int numberOfElements1{0};
    uint64_t numberOfElements2{0};
    uint64_t type{0};
    std::vector<double> pointsArray;
};

std::vector<double> meshGenerator::getMeshPointsArray() {
    return meshPointsArray;
}

int meshGenerator::getNumberOfElements() {
    return numberOfElements1;
}

unsigned int meshGenerator::getSizeOfMeshPointsArray() {
    return meshPointsArray.size();
}
#endif //MESHGENERATOR_MESHGENERATOR_H
