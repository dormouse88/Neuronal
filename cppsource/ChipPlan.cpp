/* 
 * File:   ChipPlan.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 16:57
 */

#include "ChipPlan.hpp"
#include "ChipHandle.hpp"
#include <cassert>

ChipPlan::ChipPlan(std::shared_ptr<PlanGrid> g)
    :Wirable(), planID(0), modified(false), padding(1), planGrid(g)
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
    auto oldPos = d.GetSmartPos();
    if (IsPositionFree(newPos)) d.SetPos( newPos );
    SetModified();
    PlodeRefresh(oldPos);
    PlodeRefresh(newPos);
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
    auto bound = GetWorldBound();
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

PlanRect ChipPlan::GetSmartInnerBound() const
{
    PlanRect pr;
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
        pr.SetGrid(planGrid);
        pr.tl.SetPosSmart(tl);
        pr.br.SetPosSmart(br);
        pr.valid = true;
    }
    else {
        pr.SetGrid(planGrid);
        pr.tl.SetPosSmart( VectorSmart{} );
        pr.br.SetPosSmart( VectorSmart{} );
        pr.valid = false;
    }
    return pr;
}

RectDumb ChipPlan::GetDumbBound() const
{
    auto pr = GetSmartInnerBound();
    return pr.AddPadding(padding).GetRectDumb();
}
RectWorld ChipPlan::GetWorldBound() const
{
    auto pr = GetSmartInnerBound();
    return pr.AddPadding(padding).GetRectWorld();
}


void ChipPlan::PlodeRefresh(VectorSmart point)
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

    //Plode refresh recursively back to base plan...
    auto refLock = referer.lock();
    if (refLock) {
        auto cont = refLock->GetContainer();
        cont->PlodeRefresh( refLock->GetSmartPos() );
    }
}


void ChipPlan::DrawParts(sf::RenderTarget & rt)
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

void ChipPlan::SubDraw(sf::RenderTarget & rt)
{
    auto pB = GetWorldBound();
    sf::RectangleShape planBox;
    planBox.setFillColor( sf::Color{0,50,0} );
    planBox.setOutlineColor( sf::Color{95,95,95} );
    planBox.setOutlineThickness(3.f);
    planBox.setPosition( sf::Vector2f{pB.left, pB.top} );
    planBox.setSize( sf::Vector2f{pB.width, pB.height} );
    rt.draw(planBox);
    
    sf::Text planNumText;
    planNumText.setFont( ViewResources::GetInstance().font );
    planNumText.setString( patch::to_string(GetPlanID()) );
    planNumText.setColor( sf::Color::Cyan );
    planNumText.setPosition(pB.left + pB.width/2.f, pB.top + pB.height - 40.f );
    rt.draw(planNumText);
    
    DrawParts(rt);
}

void ChipPlan::Draw(sf::RenderTarget & rt)
{
    auto pB = GetWorldBound();
    sf::RectangleShape planBox;
    planBox.setFillColor( sf::Color{50,0,0} );
    planBox.setOutlineColor( sf::Color{95,95,95} );
    planBox.setOutlineThickness(3.f);
    planBox.setPosition( sf::Vector2f{pB.left, pB.top} );
    planBox.setSize( sf::Vector2f{pB.width, pB.height} );
    rt.draw(planBox);

    DrawParts(rt);
}

