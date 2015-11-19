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


Cursor::Cursor(std::shared_ptr<ChipPlan> p, sf::Color color)
{
    ppos.SetGrid(p->GetGrid());
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
//    return posPtr ? std::make_shared<PlanPos>(*posPtr, planPtr->GetGrid()) : nullptr;
}


//void Cursor::SetPosSmart(VectorSmart newPos, std::shared_ptr<PlanGrid> newGrid)
//{
//    pposPtr = std::make_shared<PlanPos>( newPos, newGrid );
//}

//void Cursor::SetPosWorld(VectorWorld newPos, std::shared_ptr<PlanGrid> newGrid)
//{
//    {
//        pposPtr = std::make_shared<PlanPos>( newPos, newGrid );
//    }
//}

//void Cursor::SetPosSmart(VectorSmart point, std::shared_ptr<ChipPlan> plan)
//{
//    planPtr = plan;
//    posPtr = std::make_shared<VectorSmart>(point);
//}

void Cursor::SetPosWorld(VectorWorld point) //, std::shared_ptr<PlanGrid> grid
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
            auto ref = ppos.GetGrid()->GetPlan()->GetReferer();
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






//Backup//
//void Cursor::SetPosWorld(VectorWorld point, std::shared_ptr<ChipPlan> plan)
//{
//    if (planPtr == nullptr)
//    {
//        planPtr = plan;
//        Nullify();
//    }
////    if (pposPtr) { assert(planPtr->GetGrid() == pposPtr->GetGrid() ); }
//
////    auto p = std::make_shared<PlanPos> ();
//    
//    //Check if clicked pos is inside contextPlan boundary...
//    auto b = planPtr->GetWorldBound();
//    if (b.contains(point)) //(inside current context plan)
//    {
////                        p->SetGrid(planPtr->GetGrid());
////                        p->SetPosWorld(point);
////                        SetPlanPos(p);
//        
//        posPtr = std::make_shared<VectorSmart> ( planPtr->GetGrid()->MapWorldtoSmart(point) );
//        
//        //select subPlan where possible...
//        auto d = planPtr->GetDevice(*posPtr);//or GetPlanPos()->GetSmartPos());
//        if (d)
//        {
//            auto h = std::dynamic_pointer_cast<ChipHandle>(d);
//            if (h)
//            {
//                if (h->GetPlodedSize().x > 1)
//                {
//                    planPtr = h->GetPlan();
//                    Nullify();
//                    //p->SetGrid( planPtr->GetGrid() );
//                    //p->SetPosWorld( point );
//                    //SetPlanPos(p);
//                }
//            }
//        }
//    }
//    else //(not inside current context plan)
//    {
//        if (posPtr) {
//            posPtr = nullptr;
//        }
//        else {
//            auto ref = planPtr->GetReferer();
//            if (ref)
//            {
//                planPtr = ref->GetContainer();
//                auto ob = planPtr->GetWorldBound();
//                if (ob.contains(point))
//                {
////                                p->SetGrid(planPtr->GetGrid());
////                                p->SetPosWorld( point );
////                                SetPlanPos(p);
//                    posPtr = std::make_shared<VectorSmart> ( planPtr->GetGrid()->MapWorldtoSmart(point) );
//                }
//                else
//                {
//                    Nullify();
//                }
//            }
//        }
//    }
//}

