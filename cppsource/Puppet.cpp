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
{
    std::shared_ptr<ChipPlan> basePlan = BlobFactory::MakePlan();
    inner->SetSubPlan(basePlan, inner);
}

void Puppet::InitBrain()
{
    std::vector<std::string> ins;
    std::vector<std::string> outs;
    inner->DefineXputs( ins, outs );
}

void Puppet::Act()
{
    auto outs = inner->RetrieveOutputs();

    //Perform an action based on the output information...
    if (outs.at(L_FOOT) == true and outs.at(R_FOOT) == true) {
        Forward();
    }
    else if (outs.at(L_FOOT) == true) {
        Left();
    }
    else if (outs.at(R_FOOT) == true) {
        Right();
    }

    //set the new sense data from the arena...
    inner->SetInputState( L_WHISK, true );  //need to use real bool values!!!!
    inner->SetInputState( F_WHISK, true );  //need to use real bool values!!!!
}

void Puppet::Draw(sf::RenderTarget & rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {255,160,160} );
    box.setSize( sf::Vector2f{ 140.f, 140.f } );
    box.setPosition( sf::Vector2f{ 140.f, 140.f } );
    
    rt.draw(box);
}