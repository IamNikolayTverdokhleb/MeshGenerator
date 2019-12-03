#ifndef MESHGENERATOR_SOLVEROBJECT_H
#define MESHGENERATOR_SOLVEROBJECT_H
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include "meshGenerator.h"
#include <cmath>
#include "/usr/local/include/eigen3/Eigen/Dense"
using namespace Eigen;
struct borderCondition{
    int n;
    double alpha, betta, gamma;
};
class solverObject {
public:
    explicit solverObject(){
        std::cout << "Calculations started." << std::endl;
    }
    void setSystem();
    void setBorderConditions();
    void assembleSystem();
    void solveSystem();

    void printSystem();
    void fileOutputSolution();
    void visualizeSolution();

    double calculateK(const double &x, const double &y);
    double calculateF(const double &x, const double &y);
private:
    int method; /*0 - Штрафы или 1 - Лагранж*/
    int numberOfElements;
    int numberOfBorderConditions;
    double elementLength;
    std::vector<double> pointsArray;
    std::vector<borderCondition> borderConditions;
    MatrixXd globalMatrix;
    VectorXd rightSide;
    VectorXd solution;
};


#endif //MESHGENERATOR_SOLVEROBJECT_H
