#include <iostream>
#include "meshGenerator.h"
#include "solverObject.h"
int main() {
    auto *obj  = new solverObject();
    obj->setSystem();
    obj->assembleSystem();
    obj->solveSystem();
    return 0;
}