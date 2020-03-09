#ifndef MESHGENERATOR_SOLVEROBJECT_H
#define MESHGENERATOR_SOLVEROBJECT_H
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include "meshGenerator.h"
#include <cmath>
#include "/usr/local/Cellar/eigen/3.3.7/include/eigen3/Eigen/Dense"
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
    void setSystem(); /*here we read the mesh, calculate length of element(its const) and
                        initialize matrices with suitable sizes*/
    void setBorderConditions();/*here we get info about border conditions from .txt file*/
    void assembleSystem(); /*here we fill global matrix and global vector*/
    void solveSystem(); /*here we just solve the system*/

    void printSystem();/*console output for system*/
    void fileOutputSolution();/*file output (x,T). ITS IMPORTANT - bar was in (x,y), but we show solution for the length of the bar*/
    void visualizeSolution(); /*works on linux only*/

    double calculateK(const double &x, const double &y);/*here we set K(x,y)*/
    double calculateF(const double &x, const double &y);/*here we set F(x,y)*/
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
