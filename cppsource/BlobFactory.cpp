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

void BlobFactory::AddPlan()
{
    //auto mp = std::make_shared<ChipPlan> ();
}

void BlobFactory::AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight)
{
    if (plan->IsWiringFree(from, fromSlot, to, toSlot) and from.CanRegisterOut(fromSlot) and to.CanRegisterIn(toSlot))
    {
        auto mp = std::make_shared<Wire> (from, to, weight);
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

