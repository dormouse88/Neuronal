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
    :Wirable(), planID(0), modified(false), planGrid(std::make_shared<PlanGrid>())
{}

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
    if (IsPositionFree(newPos)) d.SetPos( newPos );
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
        if (x->GetSmartPos() == pos) return false;
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




std::shared_ptr<Device> ChipPlan::GetDevice(VectorSmart pos)
{
    for (auto & x: devices) {
        if (pos == x->GetSmartPos()) {
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


VectorWorld ChipPlan::GetWireAttachPos(WireAttachSide was) const
{
    sf::Vector2f wirePos;
    auto bound = GetWorldPaddedBound(2);
    //Because ChipPlans are wired internally, the wires come OUT of the left...
    if (was == WireAttachSide::OUT)
    {
        wirePos.x = bound.left;
    }
    if (was == WireAttachSide::IN)
    {
        wirePos.x = bound.left + bound.width;
    }
    wirePos.y = bound.top + (0.5 * bound.height);
    return wirePos;
}

PairVector<Smart> ChipPlan::GetSmartBound() const
{
    PairVector<Smart> pvs;
    if (devices.size() > 0)
    {
        //set initial values
        VectorSmart firstPos = devices.at(0)->GetSmartPos();
        VectorSmart tl = firstPos;
        VectorSmart br = firstPos;
        for (auto & x: devices)
        {
            VectorSmart p = x->GetSmartPos();
            if (p.x < tl.x) tl.x = p.x;
            if (p.x > br.x) br.x = p.x;
            if (p.y < tl.y) tl.y = p.y;
            if (p.y > br.y) br.y = p.y;
        }
        pvs.tl = tl;
        pvs.br = br;
        pvs.valid = true;
    }
    return pvs;
}

VectorDumb ChipPlan::GetDumbSize(int thickness) const
{
    PairVector<Smart> pvs { GetSmartBound() };
    if (pvs.valid) {
        VectorDumb tl = planGrid->MapSmartToDumb(pvs.tl);
        VectorDumb br = planGrid->MapSmartToDumb(pvs.br);
        return VectorDumb { br - tl + VectorDumb{1,1} + VectorDumb{thickness *2, thickness*2} } ;
    }
    else return VectorDumb {0,0};
}

PairVector<Dumb> ChipPlan::GetDumbPaddedBound(int thickness) const
{
    PairVector<Smart> pvs = GetSmartBound();
    if (pvs.valid) {
        pvs.AddPadding(pvs, thickness);
    }
    else {
        pvs.tl = VectorSmart { -1,-1 };
        pvs.br = VectorSmart {  1, 1 };
        pvs.valid = true;
    }
    VectorDumb tl { planGrid->MapSmartToDumb(pvs.tl) };
    VectorDumb br { planGrid->MapSmartToDumb(pvs.br) };
    return PairVector<Dumb> { tl , br };
}

RectWorld ChipPlan::GetWorldPaddedBound(int thickness) const
{
    PairVector<Smart> pvs = GetSmartBound();
    if (pvs.valid) {
        pvs.AddPadding(pvs, thickness);
    }
    else {
        pvs.tl = VectorSmart { -1,-1 };
        pvs.br = VectorSmart {  1, 1 };
        pvs.valid = true;
    }
    VectorWorld tl { planGrid->MapSmartToWorld(pvs.tl) };
    VectorWorld br { planGrid->MapSmartToWorld(pvs.br) + planGrid->WorldSizeOf(pvs.br) };
    return RectWorld { tl , br - tl };
}    


void ChipPlan::PlodeRefresh(sf::Vector2i point)
{
    int xMaxSize = 1;
    int yMaxSize = 1;
    for (auto & d: devices)
    {
        if (d->GetSmartPos().x == point.x)
        {
            if ( d->GetPlodedSize().x > xMaxSize) xMaxSize = d->GetPlodedSize().x;
        }
        if (d->GetSmartPos().y == point.y)
        {
            if ( d->GetPlodedSize().y > yMaxSize) yMaxSize = d->GetPlodedSize().y;
        }
    }
    planGrid->SetSizeX(point.x, xMaxSize);
    planGrid->SetSizeY(point.y, yMaxSize);
}


void ChipPlan::SubDraw(sf::RenderTarget & rt)
{
    for (auto & w: wires)
    {
        w->Draw(rt);
    }
    for (auto & x: devices)
    {
        x->Draw(rt);
    }
}

void ChipPlan::Draw(sf::RenderTarget & rt)
{
    auto pB = GetWorldPaddedBound(2);
    sf::RectangleShape planBox;
    planBox.setFillColor( sf::Color{50,0,0} );
    planBox.setOutlineColor( sf::Color{95,95,95} );
    planBox.setOutlineThickness(3.f);
    planBox.setPosition( sf::Vector2f{pB.left, pB.top} );
    planBox.setSize( sf::Vector2f{pB.width, pB.height} );
    rt.draw(planBox);

    SubDraw(rt);
}

