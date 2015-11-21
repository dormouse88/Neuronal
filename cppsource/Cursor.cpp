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

void Cursor::SetPosWorld(VectorWorld point)
{
    //Check if clicked pos is inside contextPlan boundary...
    auto b = ppos.GetPlan()->GetWorldBound();
    bool done = false;

    while (not b.contains(point) and not done)
    {//zoom out...
        auto ref = ppos.GetPlan()->GetHandle();
        if (ref)
        {
            ppos.SetGrid( ref->GetContainer()->GetGrid() );
            ppos.SetPosSmart( ref->GetSmartPos() );
            b = ppos.GetPlan()->GetWorldBound();
        }
        else done = true;
    }
    
    while ( not done )
    {//zoom in...
        ppos.SetPosWorld(point);
        auto d = ChipPlanFunc::GetDevice(ppos);
        auto h = std::dynamic_pointer_cast<ChipHandle>(d);
        if (h)
        {
            if (h->IsExploded())
            {
                ppos.SetGrid(h->GetPlan()->GetGrid());
            }
            else done = true;
        }
        else done = true;
    }
    b = ppos.GetPlan()->GetWorldBound();
    if (not b.contains(ppos.GetWorldPos()) ) ppos.Dislocate();
}


//void Cursor::SetPosWorld(VectorWorld point)
//{
//    //Check if clicked pos is inside contextPlan boundary...
//    auto b = ppos.GetGrid()->GetPlan()->GetWorldBound();
//    
//    if (not b.contains(point))
//    {//(not inside current context plan)
//        if (ppos.IsLocated())
//        {
//            ppos.Dislocate();
//        }
//        else
//        {
//            auto ref = ppos.GetGrid()->GetPlan()->GetHandle();
//            if (ref)
//            {
//                ppos.SetGrid( ref->GetContainer()->GetGrid() );
//                ppos.SetPosSmart( ref->GetSmartPos() );
//                                      //Zooming out directly onto other locations...
////                                    auto ob = ppos.GetGrid()->GetPlan()->GetWorldBound();
////                                    if (ob.contains(point))
////                                    {
////                                        ppos.SetPosWorld( point );
////                                    }
////                                    else
////                                    {
////                                        //(GridOnly)
////                                    }
//            }
//        }
//    }
//    else
//    {//(inside current context plan)
//        ppos.SetPosWorld(point);
//        //select subPlan where possible...
//        auto d = ppos.GetGrid()->GetPlan()->GetDevice(ppos.GetSmartPos());
//        if (d)
//        {
//            auto h = std::dynamic_pointer_cast<ChipHandle>(d);
//            if (h)
//            {
//                if (h->IsExploded())
//                {
//                    ppos.SetGrid(h->GetPlan()->GetGrid());
//                }
//            }
//        }
//    }
//}


void Cursor::Revalidate()
{
    //Just once, if the cursor is on an exploded handle, select its chip
    {
        auto d = ChipPlanFunc::GetDevice(ppos);
        auto h = std::dynamic_pointer_cast<ChipHandle>(d);
        if (h) {
            if (h->IsExploded()) ppos.SetGrid( h->GetPlan()->GetGrid() );
        }
    }
    //Then work back up (the other way) to the base
    //Ultimately select the nearest plan that is exploded all the way back.
    bool needsChanging = false;
    std::shared_ptr<ChipPlan> i = ppos.GetPlan();
    std::shared_ptr<ChipHandle> valid = nullptr;
    while (true)
    {
        auto ref = i->GetHandle();
        if (ref)
        {
            if (not ref->IsExploded())
            {
                valid = ref;
                needsChanging = true;
            }
            i = ref->GetContainer();
        }
        else break;
    }
    if (needsChanging) 
    {
        ppos.SetGrid(valid->GetContainer()->GetGrid());
        ppos.SetPosSmart(valid->GetSmartPos());
    }
}
