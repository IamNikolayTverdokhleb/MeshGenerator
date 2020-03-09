#include "solverObject.h"

void solverObject::setSystem() {
    std::string fileName = "../inputFile.txt";
    auto *obj = new meshGenerator{fileName};
    obj -> readFile();
    obj -> lineMesh();
    numberOfElements = obj -> getNumberOfElements();
    pointsArray.resize(obj->getSizeOfMeshPointsArray());
    std::copy(obj->meshPointsArray.begin(), obj-> meshPointsArray.end(),pointsArray.begin());
    delete obj;
    unsigned int size = pointsArray.size();
    double guideX = pointsArray[size - 3] - pointsArray[0];
    double guideY = pointsArray[size - 2] - pointsArray[1];
    double length = sqrt(guideX*guideX + guideY*guideY);
    elementLength = length / numberOfElements;
    std::cout <<"Number of elements: " << numberOfElements << std::endl;
    std::cout <<"Length: " << length << std::endl;
    std::cout <<"Element length: " << elementLength << std::endl;
    setBorderConditions();
    if(method == 0) {
        globalMatrix = MatrixXd::Zero(numberOfElements + 1, numberOfElements + 1);
        rightSide = VectorXd::Zero(numberOfElements + 1);
        solution = VectorXd::Zero(numberOfElements + 1);
    }
    else {
        globalMatrix = MatrixXd::Zero(numberOfElements + 1 + numberOfBorderConditions, numberOfElements + 1 + numberOfBorderConditions);
        rightSide = VectorXd::Zero(numberOfElements + 1 + numberOfBorderConditions);
        solution = VectorXd::Zero(numberOfElements + 1 + numberOfBorderConditions);
    }
}

void solverObject::assembleSystem() {

        for (int i = 0; i < numberOfElements; ++i) {
            double temp{calculateK(pointsArray[i*3], pointsArray[i*3+1]) / elementLength};
            globalMatrix(i, i) += temp;
            globalMatrix(i, i + 1) -= temp;
            globalMatrix(i + 1, i) -= temp;
            globalMatrix(i + 1, i + 1) += temp;
            rightSide(i) += calculateF(pointsArray[i*3], pointsArray[i*3+1]) *  elementLength /2.0;
            rightSide(i+1) += calculateF(pointsArray[i*3], pointsArray[i*3+1]) * elementLength /2.0;
        }
        printSystem();
    int i {0};
    for (auto &borderCondition : borderConditions) {
        double x = pointsArray[borderCondition.n*3],
                y = pointsArray[borderCondition.n*3+1];
        if(borderCondition.betta < 10e-10){
            if(method == 0) {
                    globalMatrix(borderCondition.n, borderCondition.n) +=
                            (borderCondition.alpha / borderCondition.betta) * calculateK(x,y);
                    rightSide(borderCondition.n) += (borderCondition.gamma / borderCondition.betta) * calculateK(x,y);
            }else{
                    std::cout << numberOfElements + 1 +  i << "   " << borderCondition.n << std::endl;
                    globalMatrix(numberOfElements + 1 +  i,borderCondition.n) += 1.0;
                    globalMatrix( borderCondition.n,numberOfElements  + i + 1) += 1.0;
                    rightSide(numberOfElements  + i + 1) += borderCondition.gamma / borderCondition.alpha;
                    ++i;
                  }
        }
        else{
            globalMatrix(borderCondition.n, borderCondition.n) +=
                    (borderCondition.alpha / borderCondition.betta) * calculateK(x,y);
            rightSide(borderCondition.n) += (borderCondition.gamma / borderCondition.betta) * calculateK(x,y);
        }
    }                printSystem();
        /*
    if(method == 0) {
        for (auto &borderCondition : borderConditions) {
            globalMatrix(borderCondition.n, borderCondition.n) +=
                    (borderCondition.alpha / borderCondition.betta) * calculateK(borderCondition.n*3, borderCondition.n*3);
            rightSide(borderCondition.n) += (borderCondition.gamma / borderCondition.betta) * calculateK(borderCondition.n*3, borderCondition.n*3);
        }
        printSystem();
    }else{
        int i {0};
        for (auto &borderCondition : borderConditions) {
            std::cout << numberOfElements + 1 +  i << "   " << borderCondition.n << std::endl;
            globalMatrix(numberOfElements + 1 +  i,borderCondition.n) += 1.0;
            globalMatrix( borderCondition.n,numberOfElements  + i + 1) += 1.0;
            rightSide(numberOfElements  + i + 1) += borderCondition.gamma / borderCondition.alpha;
            ++i;
        }
        printSystem();
    }*/
}

void solverObject::printSystem(){
        std::cout <<  std::endl;
        std::cout << globalMatrix << std::endl;
        std::cout << rightSide << std::endl;
        std::cout <<  std::endl;
}

double solverObject::calculateK(const double &x, const double &y) {
    return 1.0;//1+x;
}

double solverObject::calculateF(const double &x, const double &y) {
    return 0.0;//2*x + y;
}

void solverObject::setBorderConditions() {
    std::ifstream inputFile("../borderConditions.txt",std::ios_base::in);
    inputFile >> method;
    while(!inputFile.eof()) {
        borderCondition bc{};
        inputFile >> bc.n;
        inputFile >> bc.alpha;
        inputFile >> bc.betta;
        if(bc.betta == 0){bc.betta = 10e-15;}
        inputFile >> bc.gamma;
        borderConditions.push_back(bc);
    }
    inputFile.close();
    if(method == 0){std::cout << "Penalty method." <<std::endl;}
    else std::cout << "Lagrange multipliers method." << std::endl;
    std::cout << "Border conditions: " << std::endl;
    for(auto & borderCondition : borderConditions){
        std::cout << borderCondition.n << " " << borderCondition.alpha << " " << borderCondition.betta << " " << borderCondition. gamma << std::endl;
    }
    std::cout << std::endl;
    numberOfBorderConditions = borderConditions.size();
}

void solverObject::fileOutputSolution(){
    std::ofstream outputFile("../Solution.txt");
    for (int i = 0; i <= numberOfElements; ++i) {
        outputFile << i*elementLength << " " <<solution(i) << std::endl;
    }
}

void solverObject::solveSystem(){
    solution = globalMatrix.completeOrthogonalDecomposition().solve(rightSide);
    std::cout << solution << std::endl;
    fileOutputSolution();
}

void solverObject::visualizeSolution(){
    if(method == 0){
        system("rm ../PenaltySolution.eps");
        system("../visualizePenaltySolution");
        system("open ../PenaltySolution.eps");
    }
    else {
        system("rm ../LagrangeSolution.eps");
        system("../visualizeLagrangeSolution");
        system("open ../LagrangeSolution.eps");
    }
}