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
        mp->ReCalculateCharge(NULL_TAG);
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


//void BlobFactory::RemoveDevice(PlanPos pos)
//{
//    DeviceShp d = pos.GetDevice();
//    HandleShp h = pos.GetDeviceAsHandle();
//    if (not h or h->GetSubPlan()->IsEmpty())
//    {
//        pos.GetPlan()->RemoveDevice(d);
//    }
//}

//void BlobFactory::RemoveWire(Shp<WiringPair> wp)
//{
//    assert(wp and wp->from and wp->to);
//    auto wire = wp->plan->GetWire(wp);
//    if (wire)
//    {
//        Tag toSlot = wire->GetToTag();
//        wp->plan->RemoveWire(wire);
//        wire = nullptr;
//        wp->to->ReCalculateCharge(toSlot);
//    }
//}
//
