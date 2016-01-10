/* 
 * File:   BaseReferer.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:25
 */

#include "BaseReferer.hpp"

#include <iostream>
#include <set>

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
    outputsReady = false;
}
std::shared_ptr<ChipPlan> BaseReferer::GetSubPlan()
{
    return basePlan;
}



void BaseReferer::DefineXputs(XPuts all, XPutFilter filter)
{
    //this is the only thing that actually inserts into the maps
    inputs.clear();
    for (auto sd: all.ins)
    {
        if (filter == nullptr or filter->count( sd.name ) > 0)
            inputs.insert( {sd.name, sd} );
    }
    outputs.clear();
    for (auto sd: all.outs)
    {
        if (filter == nullptr or filter->count( sd.name ) > 0)
            outputs.insert( { sd.slot, sd} );
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
    for (int i = 0; i<TIME_OUT_TICKS and not outputsReady; i++)
    {
        TickOnce();
    }
    std::map<std::string, bool> retMap;
    //fill retMap with data...
    for (auto &x: outputs)
    {
        retMap.insert( {x.second.name, x.second.charge} );
        //It is necessary to clear the outputs. (Outside of the basePlan, 'true' and 'false' charges do not have parity and false is a non-response).
        x.second.charge = false;
    }
    outputsReady = false;
    return retMap;
}

