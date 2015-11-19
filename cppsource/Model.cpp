/* 
 * File:   Model.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:53
 */

#include "Model.hpp"
#include <iostream>

Model::Model()
{
    basePlan = BlobFactory::MakePlan();
}

void Model::DebugInfo()
{
//    std::cout << "MODL: " << "DEVI: " << devices.size() << ", WIRE: " << wires.size() << std::endl;
}

void Model::Logic()
{
    basePlan->PassOnCalculate();
    basePlan->PassOnAct();
}



