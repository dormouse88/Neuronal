/* 
 * File:   BlobFactory.cpp
 * Author: Dormouse
 * 
 * Created on 31 July 2015
 */

#include "BlobFactory.hpp"
#include <iostream>

BlobFactory::BlobFactory()
{}

void BlobFactory::AddNeuron(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos, int threshold)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Neuron> (serial, pos, threshold, plan);
        plan->ImportDevice(mp);
    }
}

void BlobFactory::AddJumper(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Jumper> (serial, pos, plan);
        plan->ImportDevice(mp);
    }
}

std::shared_ptr<ChipHandle> BlobFactory::AddHandle(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos)
{
    std::shared_ptr<ChipHandle> ret = nullptr;
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto handle = std::make_shared<ChipHandle> (serial, pos, plan);
        auto subPlan = MakePlan();
        subPlan->RegisterReferer(handle);
        handle->SetPlan( subPlan );
        
        plan->ImportDevice(handle);
        ret = handle;
    }
    return ret;
}

std::shared_ptr<ChipPlan> BlobFactory::MakePlan()
{
    auto g = std::make_shared<PlanGrid>();
    auto p = std::make_shared<ChipPlan>(g);
    g->RegisterPlan(p);
    return p;
}

void BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, Wirable & to, signed weight)
{
    int fromSlot = 0;
    int toSlot = 0;
    
    //Initialize toSlot to the first available valid number.
    if (to.IsSlotted(SlottedSide::IN))
    {
        toSlot = -1;
        for (int i = 1; i<=SLOT_MAX; i++)
        {
            if (to.CanRegisterIn(i))
            {
                toSlot = i;
                break;
            }
        }
    }

    //Initialize fromSlot to the first available valid number.
    if (from.IsSlotted(SlottedSide::OUT))
    {
        fromSlot = -1;
        for (int i = 1; i<=SLOT_MAX; i++)
        {
            if (from.CanRegisterOut(i))
            {
                fromSlot = i;
                break;
            }
        }
    }
    
    if (fromSlot != -1 and toSlot != -1) {
        AddWire(plan, from, fromSlot, to, toSlot, weight);
    }
}

void BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight)
{    
    //if the wire will be valid...
    if (from.HasWireTo(fromSlot, to, toSlot) == false and from.CanRegisterOut(fromSlot) and to.CanRegisterIn(toSlot) and &from != &to)
    {
        auto mp = std::make_shared<Wire> (from, fromSlot, to, toSlot, weight, plan);
        from.RegisterOut(mp);
        to.RegisterIn(mp);
        plan->ImportWire(mp);
    }
}

