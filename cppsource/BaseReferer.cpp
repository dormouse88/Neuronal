/* 
 * File:   BaseReferer.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:25
 */

#include "BaseReferer.hpp"

#include <iostream>

const int TIME_OUT_TICKS = 2000;

BaseReferer::BaseReferer()
    :outputsReady(false)
{
}


//RefererInterface...
void BaseReferer::StepOut(bool charge, int slot)
{
    if (outputs.count(slot) > 0)
    {
        outputs.at(slot).charge = charge;
        if (charge) outputsReady = true;
    }
}
void BaseReferer::SetModified()
{
    ;
}
void BaseReferer::SetSubPlan(std::shared_ptr<ChipPlan> p, std::shared_ptr<RefererInterface> myself)
{
    basePlan = p;
    p->RegisterReferer( myself );
}
std::shared_ptr<ChipPlan> BaseReferer::GetSubPlan()
{
    return basePlan;
}



void BaseReferer::DefineXputs(std::vector<std::string> ins, std::vector<std::string> outs)
{
    //this is the only thing that actually inserts into the maps
    int in_i = 1;
    for (auto x: ins)
    {
        SlotData temp { in_i, x, false };
        inputs.insert( {x, temp } );
        in_i++;
    }
    int out_i = 1;
    for (auto x: outs)
    {
        SlotData temp { out_i, x, false };
        outputs.insert( { out_i, temp} );
        out_i++;
    }
}
void BaseReferer::SetInputState(std::string name, bool charge)
{
    auto it = inputs.find(name);
    if (it != inputs.end()) it->second.charge = charge;
}
void BaseReferer::TickOnce()
{
    if (not outputsReady)
    {
        for (auto &x: inputs)
        {
            basePlan->StepIn(x.second.charge, x.second.slot);
        }
        basePlan->PassOnCalculate();
        basePlan->PassOnAct();
    }
}
std::map<std::string, bool> BaseReferer::RetrieveOutputs()
{
//    std::cout << "inputs length: " << (inputs.size()) << std::endl;
//    std::cout << "outputs length: " << (outputs.size()) << std::endl;
//    std::cout << "output counts of one: " << (outputs.count(1)) << std::endl;

    for (int i = 0; i<TIME_OUT_TICKS and not outputsReady; i++)
    {
        TickOnce();
    }
    std::map<std::string, bool> retMap;

    //fill retMap with data...
    for (auto &x: outputs)
    {
//        std::cout << "name: " << x.second.name << std::endl;
//        std::cout << "slot: " << x.second.slot << std::endl;
//        std::cout << " charge: " << x.second.charge << std::endl;
        retMap.insert( {x.second.name, x.second.charge} );
        //It is necessary to clear the outputs. (Outside of the basePlan, 'true' and 'false' charges do not have parity and false is a non-response).
        x.second.charge = false;
    }
    outputsReady = false;
    return retMap;
}

