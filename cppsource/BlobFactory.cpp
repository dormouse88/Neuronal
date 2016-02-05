/* 
 * File:   BlobFactory.cpp
 * Author: Dormouse
 * 
 * Created on 31 July 2015
 */

#include "BlobFactory.hpp"
#include "Cursor.hpp"
#include <iostream>
#include <cassert>

NeuronShp BlobFactory::AddNeuron(PlanPos pos)
{
    return AddNeuron(pos.GetPlan(), 0, pos.GetSmartPos(), 1, false);
}
NeuronShp BlobFactory::AddNeuron(PlanShp plan, int serial, VectorSmart pos, int threshold, bool hasBulb)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Neuron> (serial, pos, threshold, hasBulb, plan);
        plan->ImportDevice(mp);
        mp->Refresh(0);
        return mp;
    }
    return nullptr;
}

HandleShp BlobFactory::AddHandle(PlanPos pos)
{
    return AddHandle(pos.GetPlan(), 0, pos.GetSmartPos());
}
HandleShp BlobFactory::AddHandle(PlanShp plan, int serial, VectorSmart pos)
{
    HandleShp ret = nullptr;
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto handle = std::make_shared<ChipHandle> (serial, pos, plan);
        auto subPlan = MakePlan();
        handle->SetSubPlan( subPlan, handle );
        
        plan->ImportDevice(handle);
        ret = handle;
    }
    return ret;
}


PlanShp BlobFactory::MakePlan()
{
    auto g = std::make_shared<PlanGrid>();
    auto p = std::make_shared<ChipPlan>(g, userData_);
    g->RegisterPlan(p);
    return p;
}

std::shared_ptr<BaseReferer> BlobFactory::MakeBrain()
{
    auto brain = std::make_shared<BaseReferer>();
    PlanShp basePlan = BlobFactory::MakePlan();
    brain->SetSubPlan(basePlan, brain);
    return brain;
}



//WireShp BlobFactory::AddWire(PlanPos pos1, PlanPos pos2)
//{
//    if (ChipPlanFunc::MatchOnPlan(pos1, pos2))
//    {
//        PlanShp plan = pos1.GetPlan();
//        std::shared_ptr<Wirable> w1 = ChipPlanFunc::GetWirable( pos1 );
//        std::shared_ptr<Wirable> w2 = ChipPlanFunc::GetWirable( pos2 );
//
//        if (w1 and w2)
//        {
//            return AddWire(plan, *w1, *w2, 1);
//        }
//    }
//    return nullptr;
//}

WireShp BlobFactory::AddWire(Shp<WiringPair> wp, signed weight)
{
    //if the wire will be valid...
    if (wp->CanAddWire())
    {
        auto mp = std::make_shared<Wire> (*wp->from, wp->fromTag, *wp->to, wp->toTag, weight, wp->plan);
        wp->from->RegisterWire(InOut::OUT, mp);
        wp->to->RegisterWire(InOut::IN, mp);
        wp->plan->ImportWire(mp);
        mp->Refresh();
        return mp;
    }
    return nullptr;
}

//WireShp BlobFactory::AddWire(PlanShp plan, Wirable & from, Tag fromSlot, Wirable & to, Tag toSlot, signed weight)
    //if the wire will be valid...
//    if (from.HasWireTo(fromSlot, to, toSlot) == false and from.CanRegisterWire(InOut::OUT, fromSlot) and to.CanRegisterWire(InOut::IN, toSlot) and &from != &to)
//    {
//        auto mp = std::make_shared<Wire> (from, fromSlot, to, toSlot, weight, plan);
//        from.RegisterWire(InOut::OUT, mp);
//        to.RegisterWire(InOut::IN, mp);
//        plan->ImportWire(mp);
//        mp->Refresh();
//        return mp;
//    }
//    return nullptr;


void BlobFactory::RemoveDevice(PlanPos pos)
{
    DeviceShp d = pos.GetDevice();
    HandleShp h = pos.GetDeviceAsHandle();
    if (not h or h->GetSubPlan()->IsEmpty())
    {
        pos.GetPlan()->RemoveDevice(d);
    }
}

void BlobFactory::RemoveWire(PlanShp plan, WirableShp w1, WirableShp w2)
{
    assert(w1 and w2);
    auto wire = plan->GetWire(w1, w2);
    if (wire)
    {
        Tag toSlot = wire->GetToTag();
        plan->RemoveWire(wire);
        wire = nullptr;
        w2->Refresh(toSlot);
    }
}


