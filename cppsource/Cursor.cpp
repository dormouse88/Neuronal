/* 
 * File:   Cursor.cpp
 * Author: Dormouse
 * 
 * Created on 18 January 2015, 20:37
 */

#include "Cursor.hpp"
#include <cassert>
#include "ChipPlan.hpp"
#include "ChipHandle.hpp"


Cursor::Cursor(std::shared_ptr<PlanGrid> g, sf::Color color)
{
    ppos.SetGrid(g);
    representation.setFillColor(sf::Color::Transparent);
    representation.setOutlineColor( color );
    representation.setOutlineThickness(2.5f);
}

void Cursor::Draw(sf::RenderTarget & rt)
{
    if (ppos.IsValid())
    {
        if (not ppos.IsPlanOnly())
        {
            representation.setPosition( ppos.GetWorldPos() );
            representation.setSize( ppos.GetWorldSizeOf() );
            rt.draw(representation, sf::RenderStates(sf::BlendAdd));
        }
        else
        {
            auto b = ppos.GetPlan()->GetWorldBound();
            representation.setPosition( b.left, b.top );
            representation.setSize( sf::Vector2f { b.width, b.height } );
            rt.draw(representation, sf::RenderStates(sf::BlendAdd));
        }
    }        
}


PlanPos Cursor::GetPlanPos() const
{
    return ppos;
}

void Cursor::SetPosWorld(VectorWorld point)
{
    //Check if clicked pos is inside contextPlan boundary...
    auto b = ppos.GetGrid()->GetPlan()->GetWorldBound();
    
    if (b.contains(point)) //(inside current context plan)
    {
        ppos.SetPosWorld(point);
        
        //select subPlan where possible...
        auto d = ppos.GetGrid()->GetPlan()->GetDevice(ppos.GetSmartPos());
        if (d)
        {
            auto h = std::dynamic_pointer_cast<ChipHandle>(d);
            if (h)
            {
                if (h->IsExploded())
                {
                    ppos.SetGrid(h->GetPlan()->GetGrid());
                }
            }
        }
    }
    else //(not inside current context plan)
    {
        if (ppos.IsLocated())
        {
            ppos.SetPlanOnly();
        }
        else
        {
            auto ref = ppos.GetGrid()->GetPlan()->GetHandle();
            if (ref)
            {
                ppos.SetGrid( ref->GetContainer()->GetGrid() );
                ppos.SetPosSmart( ref->GetSmartPos() );
                                      //Zooming out directly onto other locations...
//                                    auto ob = ppos.GetGrid()->GetPlan()->GetWorldBound();
//                                    if (ob.contains(point))
//                                    {
//                                        ppos.SetPosWorld( point );
//                                    }
//                                    else
//                                    {
//                                        //(GridOnly)
//                                    }
            }
        }
    }
}

