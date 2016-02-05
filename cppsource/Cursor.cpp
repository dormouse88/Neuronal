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
    :cursorState_(CursorState::PLAN)
    ,plan_(g->GetPlan())
{
    shape_.setFillColor(sf::Color::Transparent);
    shape_.setOutlineColor( color );
    shape_.setOutlineThickness(2.5f);
}





void Cursor::Draw(sf::RenderTarget & rt)
{
    if (cursorState_ != CursorState::ABSENT)
    {
        if (cursorState_ == CursorState::PORT)
        {
            shape_.setPosition( plan_->GetGrid()->MapSmartToWorld(pos_) );
            shape_.setSize( plan_->GetGrid()->WorldSizeOf(pos_) );
            rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
        }
        if (cursorState_ == CursorState::LOCATED)
        {
            shape_.setPosition( plan_->GetGrid()->MapSmartToWorld(pos_) );
            shape_.setSize( plan_->GetGrid()->WorldSizeOf(pos_) );
            rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
        }
        else if (cursorState_ == CursorState::PLAN)
        {
            auto b = GetPlan()->GetWorldPaddedBoundBox();  //plus ports or box
            shape_.setPosition( b.left, b.top );
            shape_.setSize( sf::Vector2f { b.width, b.height } );
            rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
        }
    }        
}

void Cursor::SetPosWorld(VectorWorld point)
{
    assert(cursorState_ != CursorState::ABSENT);
    //Check if clicked pos is inside contextPlan boundary...
    RectWorld b = GetPlan()->GetWorldPaddedBoundPlusPorts();  //plus ports i think
    bool done = false;

    while (not b.contains(point) and not done)
    {//zoom out...
        auto ref = GetPlan()->GetHandle();
        if (ref)
        {
            SetToLocated( ref->GetContainer(), ref->GetSmartPos() );
            b = GetPlan()->GetWorldPaddedBoundPlusPorts();  //plus ports i think
        }
        else done = true;
    }
    
    while ( not done )
    {//zoom in...
        SetToLocated(plan_, point);

        auto h = GetPlanPos().GetDeviceAsHandle();
        if (h)
        {
            if (h->IsExploded())
            {
                SetToPlan(h->GetSubPlan());
            }
            else done = true;
        }
        else done = true;
    }
    
    VectorSmart sPos = plan_->GetGrid()->MapWorldtoSmart( point );
    PlanRegion reg = plan_->GetRegion(sPos);
    if (reg == PlanRegion::WHOLEPLAN)
    {
        SetToPlan();
    }
    else if (reg == PlanRegion::PORTS)
    {
        PortLocation port = plan_->GetPort(sPos);
        SetToPort(port);
    }
    
//    b = GetPlan()->GetWorldPaddedBoundPlusPorts();  //??

    //if point is between Plan and its Handle...
//    if ( not b.contains(point) )
//        SetToPlan();

    //if point is on a Grabber (cornerBox)...
//    RectWorld cb = { b.left, b.top, GRABBER_SIZE.x, GRABBER_SIZE.y };
//    if (cb.contains(point) ) SetToPlan();
//    cb.left = b.left + b.width - GRABBER_SIZE.x;
//    cb.top = b.top + b.height - GRABBER_SIZE.y;
//    if (cb.contains(point) ) SetToPlan();
}



void Cursor::Revalidate()
{
    //Just once, if the cursor is on an exploded handle, select its subplan
    if (GetState() == CursorState::LOCATED)
    {
        auto h = GetPlanPos().GetDeviceAsHandle();
        if (h) {
            if (h->IsExploded())
                SetToPlan( h->GetSubPlan() );
        }
    }
    //Then work back up (the other way) to the base
    //Ultimately select the nearest plan that is exploded all the way back.
    bool needsChanging = false;
    PlanShp i = GetPlan();
    HandleShp valid = nullptr;
    while (true)
    {
        auto hand = i->GetHandle();
        if (hand)
        {
            if (not hand->IsExploded())
            {
                valid = hand;
                needsChanging = true;
            }
            i = hand->GetContainer();
        }
        else break;
    }
    if (needsChanging) 
    {
        SetToLocated(i, valid->GetSmartPos());
    }
}

WirableShp Cursor::GetWirable()
{
    if (GetState() == CursorState::LOCATED)
        return GetPlan()->GetDevice( GetPlanPos().GetSmartPos() );
    if (GetState() == CursorState::PORT)
        return GetPlan();
    return nullptr;
}

PlanShp Cursor::GetParentPlan()
{
    if (GetState() != CursorState::ABSENT)
    {
        auto h = GetPlan()->GetHandle();
        if (h)
            return h->GetContainer();
    }
    return nullptr;
}


//Maybe "Match on Plan" was a better system :(
Shp<WiringPair> RetrieveWiringPair(Cursor & cu1, Cursor & cu2)
{
    //assert(cu1.GetState() != CursorState::ABSENT and cu2.GetState() != CursorState::ABSENT);
    WirableShp wib1 = cu1.GetWirable();
    WirableShp wib2 = cu2.GetWirable();
    PlanShp plan1 = cu1.GetPlan();
    PlanShp plan2 = cu2.GetPlan();
    if ( not (wib1 and wib2) )
        return nullptr;
    assert(plan1 and plan2);
    HandleShp hand1 = (cu1.GetState() == CursorState::PORT) ? plan1->GetHandle() : nullptr;
    HandleShp hand2 = (cu2.GetState() == CursorState::PORT) ? plan2->GetHandle() : nullptr;
    PlanShp parent1 = hand1 ? hand1->GetContainer() : nullptr;
    PlanShp parent2 = hand2 ? hand2->GetContainer() : nullptr;
    ZoomSide fromSide;
    ZoomSide toSide;
    
    Shp<WiringPair> ret = nullptr;
    //accept cu1 == cu2 (cu1/cu2 being PLAN or LOCATED)
    if ( plan1 and plan1 == plan2 )
    {
        ret = std::make_shared<WiringPair> ( plan1, cu1.GetWirable(), cu2.GetWirable() );
        fromSide = ZoomSide::HEAD;
        toSide   = ZoomSide::TAIL;
    }
    //accept cu1 == parent2 (only if cu2 is PLAN)
    else if ( parent2 and plan1 == parent2 )
    {
        ret = std::make_shared<WiringPair> ( plan1, cu1.GetWirable(), hand2 );
        fromSide = ZoomSide::HEAD;
        toSide   = ZoomSide::HEAD;
    }
    //accept parent1 == cu2 (only if cu1 is PLAN)
    else if ( parent1 and parent1 == plan2 )
    {
        ret = std::make_shared<WiringPair> ( plan2, hand1, cu2.GetWirable() );
        fromSide = ZoomSide::TAIL;
        toSide   = ZoomSide::TAIL;
    }
    //accept parent1 == parent2 (only if both are PLAN)
    else if ( parent1 and parent2 and parent1 == parent2 )
    {
        ret = std::make_shared<WiringPair> ( parent1, hand1, hand2 );
        fromSide = ZoomSide::TAIL;
        toSide   = ZoomSide::HEAD;
    }
    
    if ( ret and ret->from->IsSlotted(SlottedSide::OUT) )
    {
        if ( (cu1.GetState() == CursorState::PORT) and cu1.GetPort().num > 0 )
            ret->fromTag = plan1->MapPortToTag( cu1.GetPort().side, cu1.GetPort().num );
        else
            ret->fromTag = plan1->GetFirstFreeTag(fromSide);
    }
    if ( ret and ret->to->IsSlotted(SlottedSide::IN) )
    {
        if ( (cu2.GetState() == CursorState::PORT) and cu2.GetPort().num > 0 )
            ret->toTag = plan2->MapPortToTag( cu2.GetPort().side, cu2.GetPort().num );
        else
            ret->toTag = plan2->GetFirstFreeTag(toSide);
    }
    
    return ret;
    //return nullptr;
}

