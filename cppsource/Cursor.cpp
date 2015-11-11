/* 
 * File:   Cursor.cpp
 * Author: Dormouse
 * 
 * Created on 18 January 2015, 20:37
 */

#include "Cursor.hpp"
#include <cassert>
#include "ChipPlan.hpp"


Cursor::Cursor(sf::Color color)
    :pposPtr(nullptr)
{
    representation.setFillColor(sf::Color::Transparent);
    representation.setOutlineColor( color );
    representation.setOutlineThickness(2.5f);
}

void Cursor::Draw(sf::RenderTarget & rt)
{
    auto ppos = GetPlanPos();
    if (ppos) {
        representation.setPosition( ppos->GetPFPos() );
        representation.setSize( ppos->GetPFSize() );
        rt.draw(representation, sf::RenderStates(sf::BlendAdd));
    }
}

void Cursor::SetPFPos(sf::Vector2f newPos, std::shared_ptr<ChipPlan> newPlan)
{
    auto b = newPlan->GetPaddedBound();
    assert(b);
    if (b->contains(newPos))
    {
        pposPtr = std::make_shared<PlanPos>( newPos, newPlan );
    }
    else pposPtr = nullptr;
}

