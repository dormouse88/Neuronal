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
        representation.setPosition( ppos->GetWorldPos() );
        representation.setSize( ppos->GetWorldSizeOf() );
        rt.draw(representation, sf::RenderStates(sf::BlendAdd));
    }
}

void Cursor::SetPosSmart(VectorSmart newPos, std::shared_ptr<PlanGrid> newGrid)
{
    pposPtr = std::make_shared<PlanPos>( newPos, newGrid );
}
void Cursor::SetPosWorld(VectorWorld newPos, std::shared_ptr<PlanGrid> newGrid)
{
    {
        pposPtr = std::make_shared<PlanPos>( newPos, newGrid );
    }
//    else pposPtr = nullptr;
}

