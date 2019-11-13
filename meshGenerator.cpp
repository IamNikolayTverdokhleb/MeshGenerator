//
// Created by Коля on 31.10.2019.
//
#include "meshGenerator.h"
void meshGenerator::readFile() {
    std::ifstream inputFile(fileName,std::ios_base::in);
    pointsArray.reserve(12);
    double temp;

    inputFile >> numberOfDimensions; /*Считали число измерений.*/

    /*Начинаем считывание из файла.*/
    inputFile >> temp;
    pointsArray.push_back(temp);
    int i{0};
    while(true) {
        char c = inputFile.get();
        if (c == '\r' || c == '\n') {
            break;
        }
        else{
            inputFile >> temp;
            pointsArray.push_back(temp);
            i++;
        }
    }
    for (int k = 0; k < (i+1)*(numberOfDimensions - 1); ++k) {
        inputFile >> temp;
        pointsArray.push_back(temp);
    }

    /*Считали число разбиений по отрезку.*/
    inputFile >> numberOfElements1;
    if(numberOfDimensions == 4) {inputFile >> numberOfElements2;}
    /*Считали число тип разбиения.*/
    inputFile >> type;
    inputFile.close();

    /* Проверка считыванияx
    std::cout << numberOfDimensions << std::endl;
    for (int j = 0; j < 12; ++j) {
        std::cout << pointsArray[j] << std::endl;
    }
    std::cout << numberOfElements1<< std::endl;
    std::cout << numberOfElements2<< std::endl;
    std::cout << type << std::endl;
    */
}

void meshGenerator::lineMesh() {
    double guideX, guideY, guideZ; /*Направляющий вектор*/
        guideX = pointsArray[3] - pointsArray[0];
        guideY = pointsArray[4] - pointsArray[1];
        guideZ = pointsArray[5] - pointsArray[2];
    /*Вычисления точек*/
         meshPointsArray.reserve(numberOfElements1 * 3);
         for (uint64_t k = 0; k <= numberOfElements1*type; ++k) {
             meshPointsArray.push_back(pointsArray[0] + (1.0/type)*(static_cast<double>(k) / numberOfElements1) * guideX);
             meshPointsArray.push_back(pointsArray[1] + (1.0/type)*(static_cast<double>(k) / numberOfElements1) * guideY);
             meshPointsArray.push_back(pointsArray[2] + (1.0/type)*(static_cast<double>(k) / numberOfElements1) * guideZ);
         }

    /*Тестовый вывод на экран*/
    std::cout << std::endl;
    std::cout << "GuideX = " << guideX << ", GuideY = " << guideY << ", GuideZ = " << guideZ << std::endl;
    std::cout << std::endl;
    for (int k = 0; k < meshPointsArray.size(); k+=3) {
        std::cout << meshPointsArray[k]  << "  "  << meshPointsArray[k+1]<< "  " << meshPointsArray[k+2] <<std::endl;
    }
}

void meshGenerator::fileOutput() {
    std::ofstream outputFile("../MeshInfo.txt");

    if(numberOfDimensions == 2){

        outputFile << numberOfElements1 << "  " << \
            numberOfElements1 * type << "  " << 2 << "\n"; /*Число элементов - число узлов - количество контуров*/
        outputFile << 1 << "  " << 1 + type << "  ";
        if (type == 1) {
            outputFile << 1 << " " << 3 << "\n";
        }
        else{
            outputFile << 1 << " " << 4 << " " << 3 <<"\n";
        }
        int i{3};
        /*Номер элемента - сколько в нем узлов - номера узлов*/
        for (int o = 3; o <= numberOfElements1; o++) {
                outputFile << o << "  " << 1 + type << "  ";
                if (type == 1) {
                    outputFile << i << " " << i + 1 << "\n"; i++;
                }
                else{
                    outputFile << i << " " << i + 3 << " " << i+2 <<"\n"; i+=2;
                }

        }
        outputFile << 2 << "  " << 1 + type << "  ";
        if (type == 1) {
            outputFile << numberOfElements1*type + 1<< " " << 2 << "\n";
        }
        else{
            outputFile << numberOfElements1*type -1<< " " << numberOfElements1*type << " " << 2 <<"\n";
        }

        int k {0}, l{1};
        auto length = meshPointsArray.size();
        for (int j = 1; j <= length/3; ++j) {
            if(j == 2){}

            else{
                if(type == 1) {
                    outputFile << l << "  " << meshPointsArray[k] << " " << \
                meshPointsArray[k + 1] << " " << meshPointsArray[k + 2] << "\n";
                    k += 3;
                    l++;
                }
                else{
                    outputFile << l << "  " << meshPointsArray[k] << " " << \
                meshPointsArray[k + 1] << " " << meshPointsArray[k + 2] << "\n";
                    k += 3;
                    if(l % 2 == 0)
                        l--;
                    else l+=3;
                }
            }
        }
        outputFile << 2 << "  " << meshPointsArray[length-3] << " " << \
            meshPointsArray[length-2] << " " << meshPointsArray[length-1] << "\n";
        outputFile << 1 << "  " << 1 << "\n";/*Число контурных точек в каждом контуре*/
        outputFile << 1 << "\n" << 2 << "\n";/*Номера узлов контурных точек*/
    }
    outputFile.close();
}
