/* 
 * File:   ChipPlan.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 16:57
 */

#include "ChipPlan.hpp"
#include "ChipHandle.hpp"
#include <cassert>

ChipPlan::ChipPlan()
    :Wirable(), planID(0), modified(false)
{
    //dummy data for testing...
    xPansions.Insert( 4,2 );
    xPansions.Insert( -4,4 );
    yPansions.Insert( -2,2 );
}

void ChipPlan::ReceiveCharge(bool charge, int weight, int slot)
{
    auto notDead = referer.lock();
    if (notDead) notDead->StepOut(charge, slot);
}

bool ChipPlan::CanRegisterIn(int slot) const
{
    CleanWireVectors();
    for (auto w: inWires) {
        if (w.lock()->GetToSlot() == slot) return false;
    }
    return true;
}
bool ChipPlan::CanRegisterOut(int slot) const
{
    CleanWireVectors();
    for (auto w: outWires) {
        if (w.lock()->GetFromSlot() == slot) return false;
    }
    return true;
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
    if (IsPositionFree(newPos)) d.SetPIPos( newPos );
    SetModified();
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
        if (x->GetPIPos() == pos) return false;
    }
    return true;
}

void ChipPlan::ImportDevice(std::shared_ptr<Device> device)
{
    devices.emplace_back(device);
    SetModified();
}
void ChipPlan::ImportWire(std::shared_ptr<Wire> wire)
{
    wires.emplace_back(wire);
    SetModified();
}
void ChipPlan::RemoveDevice(std::shared_ptr<Device> device)
{
    if (device) {
        for (auto w : GetWires(device, true, true))
        {
            w->Zingaya();
        }
        device->Zingaya();
        CleanVectors();
        SetModified();
    }
}
void ChipPlan::RemoveWire(std::shared_ptr<Wire> wire)
{
    if (wire) {
        wire->Zingaya();
        CleanVectors();
        SetModified();
    }
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

void ChipPlan::SetModified()
{
    if (not modified)
    {
        auto refLock = referer.lock();
        if (refLock) refLock->SetModified();
        modified = true;
    }
}




std::shared_ptr<Device> ChipPlan::GetDevice(sf::Vector2i pos)
{
    //check against siblings
    for (auto & x: devices) {
        if (pos == x->GetPIPos()) {
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
    return nullptr;
}

/**
 * Returns the wire (or a null pointer) between two passed-in devices.
 * @param from device
 * @param to device
 * @return 
 */
std::shared_ptr<Wire> ChipPlan::GetWire(const Wirable& from, const Wirable& to)
{
    for (auto & x: wires) {
        if (&from == &x->GetFrom() and &to == &x->GetTo()) {
            return x;
        }
    }
    return nullptr;
}

/**
 * Returns a vector of all the wires attached to the passed-in device.
 * @param wirable the device
 * @param from include wires coming from this device
 * @param to include wires going into this device 
 * @return 
 */
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
    auto bound = GetPaddedBound();
    if (bound)
    {
        //Because ChipPlans are wired internally, the wires come OUT of the left...
        if (was == WireAttachSide::OUT)
        {
            wirePos.x = bound->left;
        }
        if (was == WireAttachSide::IN)
        {
            wirePos.x = bound->left + bound->width;
        }
        wirePos.y = bound->top + (0.5 * bound->height);
    }
    return wirePos;
}

std::shared_ptr<sf::FloatRect> ChipPlan::GetPaddedBound() const
{
    sf::Vector2i tl;
    sf::Vector2i br;
    sf::Vector2i tl_padded;
    sf::Vector2i br_padded;
    if (devices.size() > 0)
    {
        //set initial values
        auto firstPos = devices.at(0)->GetPIPos();
        tl = firstPos;
        br = firstPos;
        for (auto & x: devices)
        {
            sf::Vector2i p = x->GetPIPos();
            if (p.x < tl.x) tl.x = p.x;
            if (p.x > br.x) br.x = p.x;
            if (p.y < tl.y) tl.y = p.y;
            if (p.y > br.y) br.y = p.y;
        }
        tl_padded = tl - sf::Vector2i{ 2, 2 };
        br_padded = br + sf::Vector2i{ 2, 2 };
    }
    else //in the case of an empty plan...
    {
        tl_padded = sf::Vector2i{-1,-1};
        br_padded = sf::Vector2i{ 1, 1};
    }
    sf::Vector2f tl_f { MapPItoPF(tl_padded) };
    sf::Vector2f br_f { MapPItoPF(br_padded) + GetPFSize(br_padded) };
    return std::make_shared<sf::FloatRect> ( tl_f , br_f - tl_f );
}


void ChipPlan::Draw(sf::RenderTarget & rt)
{
    auto pB = GetPaddedBound();
    if (pB)
    {
        sf::RectangleShape planBox;
        planBox.setFillColor( sf::Color{50,0,0} );
        planBox.setOutlineColor( sf::Color{95,95,95} );
        planBox.setOutlineThickness(3.f);
        planBox.setPosition( sf::Vector2f{pB->left, pB->top} );
        planBox.setSize( sf::Vector2f{pB->width, pB->height} );
        rt.draw(planBox);
    }
    
    for (auto & w: wires)
    {
        w->Draw(rt);
    }
    for (auto & x: devices)
    {
        x->Draw(rt);
    }
}



/**
 * Maps a PlanFloat coord to a PlanInt coord
 * @param point
 * @return 
 */
sf::Vector2i ChipPlan::MapPFtoPI(const sf::Vector2f & point) const
{
    sf::Vector2i dumb {
        static_cast<int>(floorf(point.x / GRID_SIZE.x)),
        static_cast<int>(floorf(point.y / GRID_SIZE.y))
    };
    return sf::Vector2i {
        xPansions.MapDumbToSmart( dumb.x ),
        yPansions.MapDumbToSmart( dumb.y )
    };
}

/**
 * Maps a PlanInt coord to a PlanFloat coord
 * @param point
 * @return 
 */
sf::Vector2f ChipPlan::MapPItoPF(const sf::Vector2i & point) const
{
    return sf::Vector2f{
        (xPansions.MapSmartToDumb(point.x)) * GRID_SIZE.x,
        (yPansions.MapSmartToDumb(point.y)) * GRID_SIZE.y
    };
}

sf::Vector2f ChipPlan::GetPFSize(const sf::Vector2i & point) const
{
    return sf::Vector2f{
        GRID_SIZE.x * xPansions.GetSize(point.x),
        GRID_SIZE.y * yPansions.GetSize(point.y)
    };
}
