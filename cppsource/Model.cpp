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
    theSerializer = std::make_shared<Serializer> ();
}

void Model::DebugInfo()
{
//    std::cout << "MODL: " << "DEVI: " << devices.size() << ", WIRE: " << wires.size() << std::endl;
}

void Model::Logic()
{
    //all
    basePlan->PassOnCalculate();
    basePlan->PassOnAct();

    //std::cout << "Vector sizes: " << devices.size() << "  " << wires.size() << std::endl;
    //theFactory->ExpelDead();
}

void Model::SaveXML(std::shared_ptr<ChipPlan> plan)
{
    //tree
    theSerializer->SaveFile(plan); //(should save recursively, stepping in to save any unsaved children before parent)
}
void Model::LoadXML(int planNumber)
{
    theSerializer->LoadFile(theFactory, planNumber);
}


void Model::ClearEverything()
{
    //comp basePlan->clear();
}
void Model::ExpelDevices()
{
    auto remove_func = [] (std::shared_ptr<Device> eachDevice) {return eachDevice->IsDead();};
    auto new_end = std::remove_if(std::begin(devices), std::end(devices), remove_func );
    //comp devices.erase(new_end, std::end(devices) );
}
void Model::ExpelWires()
{
    auto remove_func = [] (std::shared_ptr<Wire> eachWire) {return eachWire->IsDead();};
    auto new_end = std::remove_if(std::begin(wires), std::end(wires), remove_func);
    //comp wires.erase(new_end, std::end(wires) );
}
