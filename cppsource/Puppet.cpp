/* 
 * File:   Puppet.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:05
 */

#include "Puppet.hpp"

//senses
const std::string L_WHISK = "L_WHISK";
const std::string F_WHISK = "F_WHISK";

//actions
const std::string L_FOOT = "L_FOOT";
const std::string R_FOOT = "R_FOOT";

Puppet::Puppet()
    :inner(std::make_shared<BaseReferer>())
    ,xPos(0)
    ,yPos(0)
    ,orientation(0)
{
    std::shared_ptr<ChipPlan> basePlan = BlobFactory::MakePlan();
    inner->SetSubPlan(basePlan, inner);

    InitBrain();
}

void Puppet::InitBrain()
{
    std::vector<std::string> ins;
    ins.push_back(L_WHISK);
    ins.push_back(F_WHISK);
    std::vector<std::string> outs;
    outs.push_back(L_FOOT);
    outs.push_back(R_FOOT);
    inner->DefineXputs(ins, outs);
}

void Puppet::Act()
{
    auto outs = inner->RetrieveOutputs();

    //Perform an action based on the output information...
    auto lamb = [&outs] (std::string n) { return outs.count(n) > 0 and outs.at(n) == true; };
    if (lamb(L_FOOT) and lamb(R_FOOT)) {
        Forward();
    }
    else if (lamb(L_FOOT)) {
        Left();
    }
    else if (lamb(R_FOOT)) {
        Right();
    }

    //set the new sense data from the arena...
    inner->SetInputState( L_WHISK, xPos%3==0 );
    inner->SetInputState( F_WHISK, yPos > 3 );
}

void Puppet::Draw(sf::RenderTarget & rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {255,160,160} );
    if (orientation == 0 or orientation == 2) box.setSize( sf::Vector2f{ 60.f, 140.f } );
    else box.setSize( sf::Vector2f{ 140.f, 60.f } );
    box.setPosition( sf::Vector2f{ -1600.f + xPos * 14.f, yPos * 14.f } );
    
    rt.draw(box);
}