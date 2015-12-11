/* 
 * File:   BaseReferer.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:25
 */

#include "BaseReferer.hpp"

BaseReferer::BaseReferer()
    :outputsReady(false)
{}


//RefererInterface...
void BaseReferer::StepOut(bool charge, int slot)
{
    outputs.at(slot).charge = charge;
    if (charge) outputsReady = true;
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
    for (auto &x: ins)
    {
        int i = 0;          //need to provide actual slot numbers!!!!!
        SlotData temp { i, x, false };
        inputs.insert( {x, temp } );
    }
    for (auto &x: outs)
    {
        int i = 0;          //need to provide actual slot numbers!!!!!
        SlotData temp { i, x, false };
        outputs.insert( { i, temp} );
    }
}
void BaseReferer::SetInputState(std::string name, bool charge)
{
    inputs.find(name)->second.charge = charge;
}
void BaseReferer::TickOnce()
{
    if (not outputsReady)
    {
        for (auto &x: outputs)
        {
            basePlan->ReceiveCharge(x.second.charge, 1, x.second.slot);
        }
        basePlan->PassOnCalculate();
        basePlan->PassOnAct();
    }
}
std::map<std::string, bool> BaseReferer::RetrieveOutputs()
{
    while (not outputsReady)
    {
        TickOnce();
    }
    std::map<std::string, bool> retMap;
    //fill retMap with data...
    for (auto &x: outputs) {
        retMap.insert( {x.second.name, x.second.charge} );
        //It is necessary to clear the outputs. (Outside of the basePlan, 'true' and 'false' charges do not have parity and false is a non-response).
        x.second.charge = false;
    }
    outputsReady = false;
    return retMap;
}

