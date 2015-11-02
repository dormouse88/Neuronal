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
    std::cout << "AddNeuron called" << std::endl;
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Neuron> (serial, pos, threshold);
        plan->ImportDevice(mp);
    }
}

void BlobFactory::AddSocket(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<Socket> (serial, pos);
        plan->ImportDevice(mp);
    }
}

void BlobFactory::AddHandle(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos)
{
    if (serial == 0) serial = plan->GetFreeSerial();
    if (plan->IsPositionFree(pos) and plan->IsSerialFree(serial))
    {
        auto mp = std::make_shared<ChipHandle> (serial, pos);
        plan->ImportDevice(mp);
    }
}

std::shared_ptr<ChipPlan> BlobFactory::AddPlan()
{
    return std::make_shared<ChipPlan> ();
}



void BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, Wirable & to, signed weight)
{
    int fromSlot = 0;
    int toSlot = 0;
    
    //First Initialize toSlot to the first available valid number.
    if (to.IsSlotted(SlottedSide::IN)) {
        for (int i = 1; true; i++)
        {
            if (to.CanRegisterIn(i))
            {
                toSlot = i;
                break;
            }
            //(this loop is hacky. If a Wirable won't register anything in, the loop will stick forever.)
            if (i > 200) throw "Unable to assign a slot";
        }
    }

    //Then Initialize fromSlot by incrementing fromSlot to not clash with an existing identical wire (with the already determined toSlot value).
    if (from.IsSlotted(SlottedSide::OUT)) {
        for (int i = 1; true; i++)
        {
            if (from.CanRegisterOut(i) and (from.HasWireTo(fromSlot, to, toSlot) == false))
            {
                fromSlot = i;
                break;
            }
            //(this loop is hacky. If a Wirable won't register anything in, the loop will stick forever.)
            if (i > 200) throw "Unable to assign a slot";
        }
    }
    
    AddWire(plan, from, fromSlot, to, toSlot, weight);
}

void BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight)
{    
    //WAS plan->IsWiringFree(from, fromSlot, to, toSlot)
    //if the wire will be valid...
    if (from.HasWireTo(fromSlot, to, toSlot) == false and from.CanRegisterOut(fromSlot) and to.CanRegisterIn(toSlot))
    {
        auto mp = std::make_shared<Wire> (from, fromSlot, to, toSlot, weight);
        from.RegisterOut(mp);
        to.RegisterIn(mp);
        plan->ImportWire(mp);
    }
}
//void MVCFactory::AddWire(int from, int fromSlot, int to, int toSlot, signed weight)
//{
//    auto f = model.GetDevice(from);
//    auto t = model.GetDevice(to);
//    if (f && t) AddWire(*f, fromSlot, *t, toSlot, weight);
//}

void BlobFactory::RemoveDevice(std::shared_ptr<ChipPlan> plan, std::shared_ptr<Device> device)
{
    if (plan and device) {
        for (auto w : plan->GetWires(device, true, true))
        {
            w->Zingaya();
        }
        device->Zingaya();
        plan->CleanVectors();
    }
}

void BlobFactory::RemoveWire(std::shared_ptr<ChipPlan> plan, std::shared_ptr<Wire> wire)
{
    if (wire != nullptr) {
        wire->Zingaya();
        plan->CleanVectors();
    }
}

