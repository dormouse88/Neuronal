/* 
 * File:   BlobFactory.cpp
 * Author: Dormouse
 * 
 * Created on 31 July 2015
 */

#include "BlobFactory.hpp"
#include <iostream>

std::shared_ptr<Neuron> BlobFactory::AddNeuron(PlanPos pos)
{
    if (pos.IsLocated()) return AddNeuron(pos.GetPlan(), 0, pos.GetSmartPos(), 1);
    else return nullptr;
}
std::shared_ptr<Neuron> BlobFactory::AddNeuron(std::shared_ptr<ChipPlan> plan, int serial, VectorSmart pos, int threshold)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Neuron> (serial, pos, threshold, plan);
        plan->ImportDevice(mp);
        return mp;
    }
    return nullptr;
}

std::shared_ptr<Jumper> BlobFactory::AddJumper(PlanPos pos)
{
    if (pos.IsLocated()) return AddJumper(pos.GetPlan(), 0, pos.GetSmartPos());
    else return nullptr;
}
std::shared_ptr<Jumper> BlobFactory::AddJumper(std::shared_ptr<ChipPlan> plan, int serial, VectorSmart pos)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Jumper> (serial, pos, plan);
        plan->ImportDevice(mp);
        return mp;
    }
    return nullptr;
}

std::shared_ptr<ChipHandle> BlobFactory::AddHandle(PlanPos pos)
{
    if (pos.IsLocated()) return AddHandle(pos.GetPlan(), 0, pos.GetSmartPos());
    else return nullptr;
}
std::shared_ptr<ChipHandle> BlobFactory::AddHandle(std::shared_ptr<ChipPlan> plan, int serial, VectorSmart pos)
{
    std::shared_ptr<ChipHandle> ret = nullptr;
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto handle = std::make_shared<ChipHandle> (serial, pos, plan);
        auto subPlan = MakePlan();
        //DEL//subPlan->RegisterReferer(handle);
        handle->SetSubPlan( subPlan, handle );
        
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

std::shared_ptr<BaseReferer> BlobFactory::MakeBrain()
{
    auto brain = std::make_shared<BaseReferer>();
    std::shared_ptr<ChipPlan> basePlan = BlobFactory::MakePlan();
    brain->SetSubPlan(basePlan, brain);

    //InitBrain();

    return brain;
}



std::shared_ptr<Wire> BlobFactory::AddWire(PlanPos pos1, PlanPos pos2)
{
    if (ChipPlanFunc::MatchOnPlan(pos1, pos2))
    {
        std::shared_ptr<ChipPlan> plan = pos1.GetPlan();
        std::shared_ptr<Wirable> w1 = ChipPlanFunc::GetWirable( pos1 );
        std::shared_ptr<Wirable> w2 = ChipPlanFunc::GetWirable( pos2 );

        if (w1 and w2)
        {
            return AddWire(plan, *w1, *w2, 1);
        }
    }
    return nullptr;
}

std::shared_ptr<Wire> BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, Wirable & to, signed weight)
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
        return AddWire(plan, from, fromSlot, to, toSlot, weight);
    }
    return nullptr;
}

std::shared_ptr<Wire> BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight)
{    
    //if the wire will be valid...
    if (from.HasWireTo(fromSlot, to, toSlot) == false and from.CanRegisterOut(fromSlot) and to.CanRegisterIn(toSlot) and &from != &to)
    {
        auto mp = std::make_shared<Wire> (from, fromSlot, to, toSlot, weight, plan);
        from.RegisterOut(mp);
        to.RegisterIn(mp);
        plan->ImportWire(mp);
        return mp;
    }
    return nullptr;
}


void BlobFactory::RemoveDevice(PlanPos pos)
{
    std::shared_ptr<Device> d = ChipPlanFunc::GetDevice(pos);
    if (d) {
        auto h = std::dynamic_pointer_cast<ChipHandle> (d);
        if (not h or h->GetSubPlan()->IsEmpty())
        {
            pos.GetPlan()->RemoveDevice(d);
        }
    }
}

void BlobFactory::RemoveWire(PlanPos pos1, PlanPos pos2)
{
    ChipPlanFunc::MatchOnPlan(pos1, pos2);
    std::shared_ptr<ChipPlan> plan = pos1.GetPlan();
    std::shared_ptr<Wirable> w1 = ChipPlanFunc::GetWirable( pos1 );
    std::shared_ptr<Wirable> w2 = ChipPlanFunc::GetWirable( pos2 );
    
    if (w1 and w2)
    {
        auto wire = plan->GetWire(*w1, *w2);
        if (wire) plan->RemoveWire(wire);
    }
}