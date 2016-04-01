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



BrainCursor::BrainCursor(sf::Color color)
{
    shape_.setFillColor(sf::Color::Transparent);
    shape_.setOutlineColor( color );
    shape_.setOutlineThickness(-2.5f);
}

void BrainCursor::Draw(sf::RenderTarget & rt)
{
    if (pa_.mode != PlanAddressMode::ABSENT)
    {
        PlanShp plan = pa_.plan.lock();
        if (plan)
        {
            if (pa_.mode == PlanAddressMode::PORT)
            {
                VectorSmart portPos = plan->GetPortSmartPos(pa_.port);
                shape_.setPosition( plan->GetGrid()->MapSmartToWorld( portPos ) );
                shape_.setSize( plan->GetGrid()->WorldSizeOf(portPos) );
                rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
            }
            if (pa_.mode == PlanAddressMode::CELL)
            {
                shape_.setPosition( plan->GetGrid()->MapSmartToWorld(pa_.pos) );
                shape_.setSize( plan->GetGrid()->WorldSizeOf(pa_.pos) );
                rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
            }
            if (pa_.mode == PlanAddressMode::PLANONLY)
            {
                auto b = plan->GetWorldPaddedBoundPlusPorts();  //plus ports or box
                shape_.setPosition( b.left, b.top );
                shape_.setSize( sf::Vector2f { b.width, b.height } );
                rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
            }
        }
    }
}






















Cursor::Cursor(std::shared_ptr<PlanGrid> g, sf::Color color)
    :cursorState_(CursorState::PLAN)
    //hmm//,plan_(g->GetPlan())
{
    shape_.setFillColor(sf::Color::Transparent);
    shape_.setOutlineColor( color );
    shape_.setOutlineThickness(-2.5f);
}





void Cursor::Draw(sf::RenderTarget & rt)
{
    if (cursorState_ != CursorState::ABSENT)
    {
        if (cursorState_ == CursorState::PORT)
        {
            VectorSmart portPos = plan_->GetPortSmartPos(port_);
            shape_.setPosition( plan_->GetGrid()->MapSmartToWorld( portPos ) );
            shape_.setSize( plan_->GetGrid()->WorldSizeOf(portPos) );
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
            auto b = GetPlan()->GetWorldPaddedBoundPlusPorts();  //plus ports or box
            shape_.setPosition( b.left, b.top );
            shape_.setSize( sf::Vector2f { b.width, b.height } );
            rt.draw(shape_, sf::RenderStates(sf::BlendAdd));
        }
    }        
}

void Cursor::SetToIntelligent(VectorWorld point)
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
}



void Cursor::RemainVisible()
{
    assert(GetState() != CursorState::ABSENT);
    if (plan_ != plan_->GetReferer()->GetSubPlan())
    {
        SetToPlan( plan_->GetReferer()->GetSubPlan() );
    }
    
    if (GetState() == CursorState::PORT)
    {
        if (not plan_->HasPort(port_))
            port_.num = NULL_PORT;
    }
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

//WirableShp Cursor::GetWirable()
//{
//    if (GetState() == CursorState::LOCATED)
//        return GetPlan()->GetDevice( GetPlanPos().GetSmartPos() );
//    if (GetState() == CursorState::PORT)
//        return GetPlan();
//    return nullptr;
//}

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


////Maybe "Match on Plan" was a better system :(
//Shp<WiringPair> RetrieveWiringPair(Cursor & cu1, Cursor & cu2)
//{
//    //assert(cu1.GetState() != CursorState::ABSENT and cu2.GetState() != CursorState::ABSENT);
//    WirableShp wib1 = cu1.GetWirable();
//    WirableShp wib2 = cu2.GetWirable();
//    PlanShp plan1 = cu1.GetPlan();
//    PlanShp plan2 = cu2.GetPlan();
//    if ( not (wib1 and wib2) )
//        return nullptr;
//    assert(plan1 and plan2);
//    HandleShp hand1 = (cu1.GetState() == CursorState::PORT) ? plan1->GetHandle() : nullptr;
//    HandleShp hand2 = (cu2.GetState() == CursorState::PORT) ? plan2->GetHandle() : nullptr;
//    PlanShp parent1 = hand1 ? hand1->GetContainer() : nullptr;
//    PlanShp parent2 = hand2 ? hand2->GetContainer() : nullptr;
//    ZoomSide fromSide;
//    ZoomSide toSide;
//    
//    Shp<WiringPair> ret = nullptr;
//    //accept cu1 == cu2 (cu1/cu2 being PLAN or LOCATED)
//    if ( plan1 and plan1 == plan2 )
//    {
//        ret = std::make_shared<WiringPair> ( plan1, wib1, wib2 );
//        fromSide = ZoomSide::HEAD;
//        toSide   = ZoomSide::TAIL;
//    }
//    //accept cu1 == parent2 (only if cu2 is PLAN)
//    else if ( parent2 and plan1 == parent2 )
//    {
//        ret = std::make_shared<WiringPair> ( plan1, wib1, hand2 );
//        fromSide = ZoomSide::HEAD;
//        toSide   = ZoomSide::HEAD;
//    }
//    //accept parent1 == cu2 (only if cu1 is PLAN)
//    else if ( parent1 and parent1 == plan2 )
//    {
//        ret = std::make_shared<WiringPair> ( plan2, hand1, wib2 );
//        fromSide = ZoomSide::TAIL;
//        toSide   = ZoomSide::TAIL;
//    }
//    //accept parent1 == parent2 (only if both are PLAN)
//    else if ( parent1 and parent2 and parent1 == parent2 )
//    {
//        ret = std::make_shared<WiringPair> ( parent1, hand1, hand2 );
//        fromSide = ZoomSide::TAIL;
//        toSide   = ZoomSide::HEAD;
//    }
//    
//    PortLocation port1 = cu1.GetPort();
//    PortLocation port2 = cu2.GetPort();
//    if ( ret and ret->from->IsSlotted(SlottedSide::OUT) )
//    {
//        if ( (cu1.GetState() == CursorState::PORT) and port1.num > NULL_PORT )
//            ret->fromTag = plan1->MapPortToTag( port1.side, port1.num );
//        else
//            ret->fromTag = ret->from->GetFirstFreeTag(InOut::OUT);
//    }
//    if ( ret and ret->to->IsSlotted(SlottedSide::IN) )
//    {
//        if ( (cu2.GetState() == CursorState::PORT) and port2.num > NULL_PORT )
//            ret->toTag = plan2->MapPortToTag( port2.side, port2.num );
//        else
//            ret->toTag = ret->to->GetFirstFreeTag(InOut::IN);
//    }
//    return ret;
//}

WirableShp GetWirable(PlanAddress & pa)
{
    if (pa.mode == PlanAddressMode::CELL)
        return pa.plan.lock()->GetDevice( pa.pos );
    if (pa.mode == PlanAddressMode::PORT)
        return pa.plan.lock();
    return nullptr;
}

//Maybe "Match on Plan" was a better system :(
Shp<WiringPair> RetrieveWiringPair(PlanAddress & pa1, PlanAddress & pa2)
{
    WirableShp wib1 = GetWirable(pa1);
    WirableShp wib2 = GetWirable(pa2);
    PlanShp plan1 = pa1.plan.lock();
    PlanShp plan2 = pa2.plan.lock();
    if ( not (wib1 and wib2) )
        return nullptr;
    assert(plan1 and plan2);
    HandleShp hand1 = (pa1.mode == PlanAddressMode::PORT) ? plan1->GetHandle() : nullptr;
    HandleShp hand2 = (pa2.mode == PlanAddressMode::PORT) ? plan2->GetHandle() : nullptr;
    PlanShp parent1 = hand1 ? hand1->GetContainer() : nullptr;
    PlanShp parent2 = hand2 ? hand2->GetContainer() : nullptr;
    ZoomSide fromSide;
    ZoomSide toSide;
    
    Shp<WiringPair> ret = nullptr;
    //accept cu1 == cu2 (cu1/cu2 being PLAN or LOCATED)
    if ( plan1 and plan1 == plan2 )
    {
        ret = std::make_shared<WiringPair> ( plan1, wib1, wib2 );
        fromSide = ZoomSide::HEAD;
        toSide   = ZoomSide::TAIL;
    }
    //accept cu1 == parent2 (only if cu2 is PLAN)
    else if ( parent2 and plan1 == parent2 )
    {
        ret = std::make_shared<WiringPair> ( plan1, wib1, hand2 );
        fromSide = ZoomSide::HEAD;
        toSide   = ZoomSide::HEAD;
    }
    //accept parent1 == cu2 (only if cu1 is PLAN)
    else if ( parent1 and parent1 == plan2 )
    {
        ret = std::make_shared<WiringPair> ( plan2, hand1, wib2 );
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
    
    PortLocation port1 = pa1.port;
    PortLocation port2 = pa2.port;
    if ( ret and ret->from->IsSlotted(SlottedSide::OUT) )
    {
        if ( (pa1.mode == PlanAddressMode::PORT) and port1.num > NULL_PORT )
            ret->fromTag = plan1->MapPortToTag( port1.side, port1.num );
        else
            ret->fromTag = ret->from->GetFirstFreeTag(InOut::OUT);
    }
    if ( ret and ret->to->IsSlotted(SlottedSide::IN) )
    {
        if ( (pa2.mode == PlanAddressMode::PORT) and port2.num > NULL_PORT )
            ret->toTag = plan2->MapPortToTag( port2.side, port2.num );
        else
            ret->toTag = ret->to->GetFirstFreeTag(InOut::IN);
    }
    return ret;
}









void MakeValidAndVisible(PlanAddress & pa)
{
    PlanShp plan = pa.plan.lock();
    if (pa.mode != PlanAddressMode::ABSENT and plan)
    {
        if (plan != plan->GetReferer()->GetSubPlan())
            plan = plan->GetReferer()->GetSubPlan();
        if (pa.mode == PlanAddressMode::PORT)
        {
            if (not plan->HasPort(pa.port))
                pa.port.num = NULL_PORT;
        }

        //Just once, if the cursor is on an exploded handle, select its subplan
        if (pa.mode == PlanAddressMode::CELL)
        {
            DeviceShp d = plan->GetDevice( pa.pos );
            HandleShp h = std::dynamic_pointer_cast<ChipHandle>(d);
            if (h and h->IsExploded())
            {
                pa.mode = PlanAddressMode::PLANONLY;
                plan = h->GetSubPlan();
            }
        }
        //Then work back up (the other way) to the base
        //Ultimately select the nearest plan that is exploded all the way back.
        HandleShp newAddressee = nullptr;
        while (true)
        {
            auto hand = plan->GetHandle();
            if (hand)
            {
                if (not hand->IsExploded())
                {
                    newAddressee = hand;
                }
                plan = hand->GetContainer();
            }
            else break;
        }
        pa.plan = plan;
        if (newAddressee)
        {
            pa.mode = PlanAddressMode::CELL;
            pa.pos = newAddressee->GetSmartPos();
        }
    }
}

