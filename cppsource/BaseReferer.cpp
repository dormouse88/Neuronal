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
//void BaseReferer::StepOut(bool charge, int slot)
//{
//    if (outputs.count(slot) > 0)
//    {
//        outputs.at(slot).charge = charge;
//        if (charge) outputsReady = true;
//    }
//}
void BaseReferer::StepOutRefresh(int slot)
{
    
}
bool BaseReferer::StepOutGetOutgoingCharge(int slot)
{
    
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
        if (filter == nullptr or filter->count( sd.name ) > 0 or sd.name == "WAIT") //bit naughty but who cares
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
            //basePlan->StepIn(x.second.charge, x.second.slot);
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
        //It is seemingly not necessary to clear the outputs. (Outside of the basePlan, 'true' and 'false' charges do not have parity and false is a non-response).
        //Not clearing them allows Draw() to highlight outputs activated.
        //x.second.charge = false;
    }
    outputsReady = false;
    return retMap;
}

void BaseReferer::DrawBrain(sf::RenderTarget & rt)
{
    RectWorld bound = basePlan->GetWorldPaddedBound();
    float xi = bound.left - 20.f;
    float yi = bound.top + 20.f; 
    float xo = bound.left + bound.width + 20.f;
    float yo = bound.top + 20.f; 

    const sf::Color BRIGHT { 0, 255, 0 };
    const sf::Color DULL   { 0, 125, 0 };
    sf::Text t;
    t.setFont(ViewResources::GetInstance().font);
    t.setCharacterSize(22.f);
    for (auto p: inputs) {
        if (p.second.charge) t.setColor( BRIGHT );
        else t.setColor( DULL );
        t.setString( patch::to_string(p.second.slot) + ": " + p.second.name );
        t.setPosition(xi - t.getGlobalBounds().width, yi);
        rt.draw(t);
        yi += 70.f;
    }
    for (auto p: outputs) {
        if (p.second.charge) t.setColor( BRIGHT );
        else t.setColor( DULL );
        t.setString( patch::to_string(p.second.slot) + ": " + p.second.name );
        t.setPosition(xo, yo);
        rt.draw(t);
        yo += 70.f;
    }
    
    basePlan->Draw(rt);
}
