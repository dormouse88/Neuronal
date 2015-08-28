/* 
 * File:   ChipPlan.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 16:57
 */

#include "ChipPlan.hpp"
#include "ChipHandle.hpp"

ChipPlan::ChipPlan()
    :Wirable()
{}

void ChipPlan::ReceiveCharge(bool charge, int weight, int slot)
{
    auto notDead = referer.lock();
    notDead->StepOut(charge, slot);
}

void ChipPlan::StepIn(bool charge, int slot)
{
    PushCharge(charge, slot);
}

void ChipPlan::PassOnAct()
{
    for (auto & d: devices) d->LogicAct();
}
void ChipPlan::PassOnCalculate()
{
    for (auto & d: devices) d->LogicCalculate();
}

/////////////////////////////


void ChipPlan::SetPosition(Device & d, sf::Vector2i newPos)
{
    if (IsPositionFree(newPos)) d.SetPosInPlan( newPos );
}
int ChipPlan::GetFreeSerial() const
{
    for (int i = 1; true; i++)
    {
        if (IsSerialFree(i)) return i;
    }
}
bool ChipPlan::IsSerialFree(int serial) const
{
    for (const auto & x : devices) {
        if (x->GetSerial() == serial) return false;
    }
    return true;
}
bool ChipPlan::IsPositionFree(sf::Vector2i pos) const
{
    for (const auto & x : devices) {
        if (x->GetPosInPlan() == pos) return false;
    }
    return true;
}
bool ChipPlan::IsWiringFree(Wirable & from, int fromSlot, Wirable & to, int toSlot) const
{
    //check against siblings
    for (const auto & x: wires) {
        if ( (&x->GetFrom() == &from and x->GetFromSlot() == fromSlot) and (&x->GetTo() == &to and x->GetToSlot() == toSlot) ) return false;
    }
    return true;
}


void ChipPlan::ImportDevice(std::shared_ptr<Device> device)
{
    devices.emplace_back(device);
}
void ChipPlan::ImportWire(std::shared_ptr<Wire> wire)
{
    wires.emplace_back(wire);
}

void ChipPlan::CleanVectors()
{
    {
        auto remove_func = [] (std::shared_ptr<Device> eachDevice) {return eachDevice->IsDead();};
        auto new_end = std::remove_if(std::begin(devices), std::end(devices), remove_func );
        devices.erase(new_end, std::end(devices) );
    }
    {
        auto remove_func = [] (std::shared_ptr<Wire> eachWire) {return eachWire->IsDead();};
        auto new_end = std::remove_if(std::begin(wires), std::end(wires), remove_func);
        wires.erase(new_end, std::end(wires) );
    }
}



std::shared_ptr<Device> ChipPlan::GetDevice(sf::Vector2i pos)
{
    //check against siblings
    for (auto & x: devices) {
        if (pos == x->GetPosInPlan()) {
            return x;
        }
    }
    return nullptr;
}
std::shared_ptr<Device> ChipPlan::GetDevice(int serial)
{
    for (auto & x: devices) {
        if (serial == x->GetSerial()) {
            return x;
        }
    }
}
std::shared_ptr<Wire> ChipPlan::GetWire(const Wirable& from, const Wirable& to)
{
    for (auto & x: wires) {
        if (&from == &x->GetFrom() and &to == &x->GetTo()) {
            return x;
        }
    }
    return nullptr;
}
std::vector<std::shared_ptr<Wire> > ChipPlan::GetWires(std::shared_ptr<Wirable> wirable, bool from, bool to)
{
    std::vector<std::shared_ptr<Wire> > ret_vec;
    for (auto & x: wires) {
        if ((from and wirable.get() == &x->GetFrom()) or (to and wirable.get() == &x->GetTo())) {
            ret_vec.emplace_back(x);
        }
    }
    return ret_vec;
}


sf::Vector2f ChipPlan::GetWireAttachPos(WireAttachSide was) const
{
    sf::Vector2f wirePos;
    //erm......
    return wirePos;
}


void ChipPlan::Draw(sf::RenderTarget & rt)
{
    //(Plan should draw itself here)
    
    for (auto & w: wires)
    {
        w->Draw(rt);
    }
    for (auto & x: devices)
    {
        x->Draw(rt);
    }
}
void ChipPlan::Handle(int code)
{
    if (code == 1) {
        ;
    }
}
