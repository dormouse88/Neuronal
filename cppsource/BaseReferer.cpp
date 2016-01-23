/* 
 * File:   BaseReferer.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:25
 */

#include "BaseReferer.hpp"

#include <iostream>
#include <set>

BaseReferer::BaseReferer()
{}

void BaseReferer::StepOutRefresh(Tag slot)
//outputs may have changed
{
    RefreshOutputs();
}

bool BaseReferer::StepOutGetOutgoingCharge(Tag slot)
//return outgoing charges from inputs
{
    for (auto &x: inputs_)
    {
        if ( x.second.slot == slot )
            return x.second.charge;
    }
    return false;
}


void BaseReferer::SetModified()
{
    ;
}
void BaseReferer::SetSubPlan(PlanShp p, std::shared_ptr<RefererInterface> myself)
{
    subPlan_ = p;
    p->RegisterReferer( myself );
    RefreshOutputs();
}
PlanShp BaseReferer::GetSubPlan()
{
    return subPlan_;
}



void BaseReferer::DefineXputs(XPuts all, XPutFilter filter)
{
    //this is the only thing that actually inserts into the maps
    inputs_.clear();
    for (auto sd: all.ins)
    {
        if (filter == nullptr or filter->count( sd.name ) > 0)
            inputs_.insert( {sd.name, sd} );
    }
    outputs_.clear();
    for (auto sd: all.outs)
    {
        if (filter == nullptr or filter->count( sd.name ) > 0 or sd.name == "WAIT") //bit naughty but who cares
            outputs_.insert( { sd.slot, sd} );
    }
}
void BaseReferer::SetInputState(std::string name, bool charge)
{
    auto it = inputs_.find(name);
    if (it != inputs_.end())
    {
        if (it->second.charge != charge)
        {
            it->second.charge = charge;
            subPlan_->StepInRefresh(it->second.slot);
        }
    }
}
void BaseReferer::TickOnce()
{
    subPlan_->PassOnCalculate();
    subPlan_->PassOnAct();
}

std::map<std::string, bool> BaseReferer::GetOutputs()
{
    std::map<std::string, bool> retMap;
    //fill retMap with data...
    for (auto &x: outputs_)
    {
        retMap.insert( {x.second.name, x.second.charge} );
        //It is seemingly not necessary to clear the outputs. (Outside of the basePlan, 'true' and 'false' charges do not have parity and false is a non-response).
        //Not clearing them allows Draw() to highlight outputs activated.
        //    //x.second.charge = false;
    }
    return retMap;
}
bool BaseReferer::IsAnyOutputOn() const
{
    for (auto x: outputs_)
    {
        if (x.second.charge)
            return true;
    }
    return false;
}

void BaseReferer::RefreshOutputs()
{
    for (auto & x: outputs_)
    {
        x.second.charge = subPlan_->StepInGetOutgoingCharge(x.second.slot);
    }
}


void BaseReferer::DrawBrain(sf::RenderTarget & rt)
{
    RectWorld bound = subPlan_->GetWorldPaddedBoundPlusPorts();
    float xi = bound.left - 20.f;
    float yi = bound.top + 20.f; 
    float xo = bound.left + bound.width + 20.f;
    float yo = bound.top + 20.f; 

    const sf::Color BRIGHT { 0, 255, 0 };
    const sf::Color DULL   { 0, 125, 0 };
    sf::Text t;
    t.setFont(ViewResources::GetInstance().font);
    t.setCharacterSize(22.f);
    for (auto p: inputs_) {
        if (p.second.charge) t.setColor( BRIGHT );
        else t.setColor( DULL );
        t.setString( patch::to_string(p.second.slot) + ": " + p.second.name );
        t.setPosition(xi - t.getGlobalBounds().width, yi);
        rt.draw(t);
        yi += 70.f;
    }
    for (auto p: outputs_) {
        if (p.second.charge) t.setColor( BRIGHT );
        else t.setColor( DULL );
        t.setString( patch::to_string(p.second.slot) + ": " + p.second.name );
        t.setPosition(xo, yo);
        rt.draw(t);
        yo += 70.f;
    }
    
    subPlan_->Draw(rt);
}
