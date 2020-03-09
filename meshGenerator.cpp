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

    // Проверка считывания
    std::cout << "Number of dimensions: " << numberOfDimensions << std::endl;
    /*std::cout << std::endl;
    for (int j = 0; j < 12; ++j) {
        std::cout << pointsArray[j] << std::endl;
    }
    std::cout << std::endl;*/
    std::cout << "Number of elements 1: " << numberOfElements1<< std::endl;
    std::cout << "Number of elements 2: " << numberOfElements2<< std::endl;
    std::cout << "Type of element: " << type << std::endl;

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
    /*std::cout << std::endl;
    std::cout << "GuideX = " << guideX << ", GuideY = " << guideY << ", GuideZ = " << guideZ << std::endl;
    std::cout << std::endl;*/
    for (int k = 0; k < meshPointsArray.size(); k+=3) {
        std::cout << meshPointsArray[k]  << "  "  << meshPointsArray[k+1]<< "  " << meshPointsArray[k+2] <<std::endl;
    }
}

void meshGenerator::fileOutput() {
    std::ofstream outputFile("../MeshInfo.txt");

    if(numberOfDimensions == 2){
        auto length = meshPointsArray.size();
        outputFile << numberOfElements1 << "  " << \
            length / 3<< "  " << 2 << "\n"; /*Число элементов - число узлов - количество контуров*/
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
            outputFile << numberOfElements1*type -1<< " " << numberOfElements1*type +1 << " " << 2 <<"\n";
        }

        int k {0}, l{1};

        for (int j = 1; j <= length/3; ++j) {
            if(j == 2){
                if(type==1){l++;}
                else{}
            }

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
                   if(l > length/3){
                       l -= 1;
                   }
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

void meshGenerator::twoDimensionalMesh() {
    meshPointsArray.reserve((numberOfElements1 + numberOfElements2)*6);/*Two sides with same amount of points, {x,y,z}*/
    /*this creates countur points*/
    system("rm ../Mesh.txt");
    for (int i = 0; i < 4; ++i) {
        double guideX, guideY, guideZ; /*Направляющий вектор*/
        if(i!= 3) {
            guideX = pointsArray[i * 3 + 3] - pointsArray[i * 3];
            guideY = pointsArray[i * 3 + 4] - pointsArray[i * 3 + 1];
            guideZ = pointsArray[i * 3 + 5] - pointsArray[i * 3 + 2];
            if(i == 0 || i == 2) {
                for (uint64_t k = 0; k <= numberOfElements1; ++k) {
                    meshPointsArray.push_back(
                            pointsArray[i * 3] + (static_cast<double>(k) / numberOfElements1) * guideX);
                    meshPointsArray.push_back(
                            pointsArray[i * 3 + 1] + (static_cast<double>(k) / numberOfElements1) * guideY);
                    meshPointsArray.push_back(
                            pointsArray[i * 3 + 2] + (static_cast<double>(k) / numberOfElements1) * guideZ);
                }
            }
            else{
                for (uint64_t k = 1; k < numberOfElements2; ++k) {
                    meshPointsArray.push_back(
                            pointsArray[i * 3] + (static_cast<double>(k) / numberOfElements2) * guideX);
                    meshPointsArray.push_back(
                            pointsArray[i * 3 + 1] + (static_cast<double>(k) / numberOfElements2) * guideY);
                    meshPointsArray.push_back(
                            pointsArray[i * 3 + 2] + (static_cast<double>(k) / numberOfElements2) * guideZ);
                }
            }
        }
        if(i == 3){
            guideX = pointsArray[0] - pointsArray[i*3];
            guideY = pointsArray[1] - pointsArray[i*3 + 1];
            guideZ = pointsArray[2] - pointsArray[i*3 + 2];
            for (uint64_t k = 1; k < numberOfElements2; ++k) {
                meshPointsArray.push_back(
                        pointsArray[i * 3] + (static_cast<double>(k) / numberOfElements2) * guideX);
                meshPointsArray.push_back(
                        pointsArray[i * 3 + 1] + (static_cast<double>(k) / numberOfElements2) * guideY);
                meshPointsArray.push_back(
                        pointsArray[i * 3 + 2] + (static_cast<double>(k) / numberOfElements2) * guideZ);
            }
        }
    }
   /* std::ofstream CoutputFile("../ContourMesh.txt");
    int length1 = meshPointsArray.size();
    std:: cout << " SIZE = " << meshPointsArray.size() << std::endl;
    for (int j = 0; j < length1; j+=3) {
        CoutputFile << meshPointsArray[j] << " " << meshPointsArray[j+1] << " " << meshPointsArray[j+2] << " --- " << j/3 << std::endl;
    }
    CoutputFile.close();*/
    /*and here we connect all points*/
    double guideX, guideY, guideZ;
    for (int i = 1; i < numberOfElements1; ++i) {
        int index = 2*numberOfElements1  + numberOfElements2 - i;
        guideX = meshPointsArray[index*3] - meshPointsArray[i * 3];
        guideY = meshPointsArray[index*3 + 1] -meshPointsArray[i * 3 + 1];
        guideZ = meshPointsArray[index*3 + 2] -meshPointsArray[i * 3 + 2];
        for (int j = 1; j < numberOfElements2; ++j) {
            meshPointsArray.push_back(
                    meshPointsArray[i * 3] + (static_cast<double>(j) / numberOfElements2) * guideX);
            meshPointsArray.push_back(
                    meshPointsArray[i * 3 + 1] + (static_cast<double>(j) / numberOfElements2) * guideY);
            meshPointsArray.push_back(
                    meshPointsArray[i * 3 + 2] + (static_cast<double>(j) / numberOfElements2) * guideZ);
        }
    }
    std::ofstream outputFile("../Mesh.txt");
    int length = meshPointsArray.size();
    for (int j = 0; j < length; j+=3) {
        outputFile << meshPointsArray[j] << " " << meshPointsArray[j+1] << " " << meshPointsArray[j+2] << std::endl;
    }
    outputFile.close();
    /*std::ofstream CNEWoutputFile("../FUllMesh.txt");
    int length1 = meshPointsArray.size();
    std:: cout << " SIZE = " << meshPointsArray.size() << std::endl;
    for (int j = 0; j < length1; j+=3) {
        CNEWoutputFile << meshPointsArray[j] << " " << meshPointsArray[j+1] << " " << meshPointsArray[j+2] << " --- " << j/3 << std::endl;
    }
    CNEWoutputFile.close();*/
    system("rm ../Mesh.eps");
    system("../visualizeMesh");
    system("open ../Mesh.eps");
   /* acces to 12 club ;
    intel core i 5 влзомать сейчас же компик!
    vzlom mac os catalina 112""
    script 2.01 4 15
    gg vp active
    play tab 2 fuck ur mom
    i had sex with ur
    comp не моэет произвести операция под кодом 2ю2ю
    comand shift tub
    взломать пентагон. 202 ошибка
    пропись кода в мак ойс
    ДОЛБАЕБАМ ТУТ НЕ МЕСТО!
*/
}


