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
private:
    std::string fileName;
    uint64_t numberOfDimensions{0};
    uint64_t numberOfElements1{0};
    uint64_t numberOfElements2{0};
    uint64_t type{0};
    std::vector<double> pointsArray;
    std::vector<double> meshPointsArray;
};


#endif //MESHGENERATOR_MESHGENERATOR_H
