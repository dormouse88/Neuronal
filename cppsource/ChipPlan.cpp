/* 
 * File:   ChipPlan.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 16:57
 */

#include "ChipPlan.hpp"
#include "ChipHandle.hpp"
#include <cassert>
#include "UserData.hpp" //fwd dec

ChipPlan::ChipPlan(std::shared_ptr<PlanGrid> g, std::shared_ptr<UserData> u)
    :Wirable(), planID(0), modified(false), padding(2), planGrid(g), userData_(u)
{
    RecalculateSmartInnerBound();
}

void ChipPlan::RegisterReferer(std::shared_ptr<RefererInterface> ref)
{
    referer = ref;
}
std::shared_ptr<RefererInterface> ChipPlan::GetReferer()
{
    return referer;
}
std::shared_ptr<ChipHandle> ChipPlan::GetHandle()
{
    return std::dynamic_pointer_cast<ChipHandle>( referer );
}



void ChipPlan::Refresh(int slot)
{
    if (referer)
        referer->StepOutRefresh(slot);
}
bool ChipPlan::GetOutgoingCharge(int slot)
{
    if (referer)
        return referer->StepOutGetOutgoingCharge(slot);
    return false;
}



bool ChipPlan::CanRegisterIn(int slot) const
{
    return IsInSlotFree(slot);
}
bool ChipPlan::CanRegisterOut(int slot) const
{
    return IsOutSlotFree(slot);
}


void ChipPlan::StepInRefresh(int slot)
{
    PropagateRefresh(slot);
}
bool ChipPlan::StepInGetOutgoingCharge(int slot)
{
    GetTotalIncomingCharge(slot);
}

void ChipPlan::PassOnAct()
{
    for (auto & d: devices) d->InnerStep();
}
void ChipPlan::PassOnCalculate()
{
    for (auto & d: devices) d->PreInnerStep();
}

/////////////////////////////


void ChipPlan::SetPosition(Device & d, VectorSmart newPos)
{
    auto oldPos = d.GetSmartPos();
    if (IsPositionFree(newPos)) d.SetPosSmart( newPos );
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
bool ChipPlan::IsPositionFree(VectorSmart pos) const
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
    RecalculateSmartInnerBound();
    
    if (not modified)
    {
        if (referer)
            referer->SetModified();
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
    auto bound = GetWorldPaddedBound();
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

void ChipPlan::RecalculateSmartInnerBound()
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
    if (not (pr == smartInnerBound))
    {
        smartInnerBound = pr;
        PlodeRefresh();
    }
}
PlanRect ChipPlan::GetSmartInnerBound() const
{
    return smartInnerBound;
}
PlanRect ChipPlan::GetSmartPaddedBound() const
{
    PlanRect b = smartInnerBound; //make a copy (AddPadding is not const so can't be called on the member)
    return b.AddPadding(padding);
}
RectDumb ChipPlan::GetDumbPaddedBound() const
{
    auto pr = GetSmartPaddedBound();
    return pr.GetRectDumb();
}
RectWorld ChipPlan::GetWorldPaddedBound() const
{
    auto pr = GetSmartPaddedBound();
    return pr.GetRectWorld();
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
    //Propagate recursively...
    PlodeRefresh();
}

void ChipPlan::PlodeRefresh()
{
    //Plode refresh recursively back to base plan...
    auto refLock = GetHandle();
    if (refLock) {
        auto cont = refLock->GetContainer();
        cont->PlodeRefresh( refLock->GetSmartPos() );
    }
}

void ChipPlan::DrawBox(sf::RenderTarget & rt)
{
    sf::Color randCol {
         static_cast<sf::Uint8>( ((GetPlanID()+ 130) * 20000) % 255 )
        ,static_cast<sf::Uint8>( ((GetPlanID()+ 130) * 22000) % 255 )
        ,static_cast<sf::Uint8>( ((GetPlanID()+ 130) * 25000) % 255 )
    };
    if (IsEmpty()) randCol = sf::Color::Black;
    auto pB = GetWorldPaddedBound();
    sf::RectangleShape planBox;
    planBox.setFillColor( randCol * sf::Color{70,70,70} );
    planBox.setOutlineColor( sf::Color{95,95,95} );
    planBox.setOutlineThickness(6.f);
    planBox.setPosition( sf::Vector2f{pB.left, pB.top} );
    planBox.setSize( sf::Vector2f{pB.width, pB.height} );
    rt.draw(planBox);
    
    //Grabber
    sf::RectangleShape cornerBox;
    cornerBox.setSize( GRABBER_SIZE );
    cornerBox.setPosition( planBox.getPosition() );
    cornerBox.setFillColor( sf::Color{95,95,95} );
    rt.draw(cornerBox);
    cornerBox.setSize( GRABBER_SIZE );
    cornerBox.setPosition( pB.left + pB.width - GRABBER_SIZE.x, pB.top + pB.height - GRABBER_SIZE.y );
    rt.draw(cornerBox);
}
void ChipPlan::DrawTitle(sf::RenderTarget & rt)
{
    auto pB = GetWorldPaddedBound();
    sf::Text planNumText;
    planNumText.setFont( ViewResources::GetInstance().font );
    std::string textString { patch::to_string(GetPlanID()) };
    if (userData_) {
        textString.append(" -- " + userData_->GetNameByID(GetPlanID()));
        auto rels = userData_->GetRelatives(GetPlanID());
        if (rels and rels->parent) textString.append(" // " + patch::to_string(rels->parent) + " ");
    }
    if (IsModified()) {
        textString.append("**");
        textString.insert(0, "**");
    }
    planNumText.setString( textString );
    planNumText.setColor( sf::Color::Cyan );
    planNumText.setPosition(pB.left + pB.width/2.f, pB.top + pB.height - 40.f );
    rt.draw(planNumText);
}

void ChipPlan::DrawGridLines(sf::RenderTarget & rt)
{
    if (IsEmpty()) return;
    sf::Color lineCol {14,24,34};
    auto pB = GetWorldPaddedBound();
    PlanRect r = GetSmartPaddedBound();
    for (int i = r.tl.GetSmartPos().y + 1; i <= r.br.GetSmartPos().y; i++)
    {
        std::vector<sf::Vertex> line;
        sf::Vector2f posFrom    { pB.left, planGrid->MapSmartToWorld({0,i}).y };
        line.push_back( sf::Vertex( posFrom, lineCol ) );
        sf::Vector2f posTo      { pB.left + pB.width, planGrid->MapSmartToWorld({0,i}).y }; 
        line.push_back( sf::Vertex(posTo, lineCol ) );
        rt.draw(&line[0], line.size(), sf::Lines, sf::RenderStates(sf::BlendAdd));
    }
    for (int i = r.tl.GetSmartPos().x + 1; i <= r.br.GetSmartPos().x; i++)
    {
        std::vector<sf::Vertex> line;
        sf::Vector2f posFrom    { planGrid->MapSmartToWorld({i,0}).x, pB.top };
        line.push_back( sf::Vertex( posFrom, lineCol ) );
        sf::Vector2f posTo      { planGrid->MapSmartToWorld({i,0}).x, pB.top + pB.height }; 
        line.push_back( sf::Vertex(posTo, lineCol ) );
        rt.draw(&line[0], line.size(), sf::Lines, sf::RenderStates(sf::BlendAdd));
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
    DrawBox(rt);
    DrawGridLines(rt);
    DrawTitle(rt);
    DrawParts(rt);
}

void ChipPlan::Draw(sf::RenderTarget & rt)
{
    SubDraw(rt);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////namespace

std::shared_ptr<Device> ChipPlanFunc::GetDevice(PlanPos pos)
{
    if (pos.IsLocated()) return pos.GetPlan()->GetDevice(pos.GetSmartPos());
    else return nullptr;
}
std::shared_ptr<Wirable> ChipPlanFunc::GetWirable(PlanPos pos)
{
    if (pos.IsLocated()) return GetDevice(pos);
    else return pos.GetPlan();
}
std::shared_ptr<Wire> ChipPlanFunc::GetWire(PlanPos pos1, PlanPos pos2)
{
    if (MatchOnPlan(pos1, pos2))  //(pos1.GetGrid() == pos2.GetGrid())
    {
        auto w1 = GetWirable(pos1);
        auto w2 = GetWirable(pos2);
        if (w1 and w2) return pos1.GetPlan()->GetWire(*w1, *w2);
    }
    return nullptr;
}
std::vector<std::shared_ptr<Wire> > ChipPlanFunc::GetWires(PlanPos pos, bool from, bool to)
{
    auto w1 = GetWirable(pos);
    if (w1) return pos.GetPlan()->GetWires(w1, from, to);
    return std::vector<std::shared_ptr<Wire> >();
}

bool ChipPlanFunc::IsPositionFree(PlanPos pos)
{
    return pos.GetPlan()->IsPositionFree(pos.GetSmartPos());
}
void ChipPlanFunc::SetPosition(PlanPos dPos, PlanPos toPos)
{
    if ( MatchOnPlan(dPos, toPos) )
    {
        if ( dPos.IsLocated() and toPos.IsLocated() )
        {
            auto d = GetDevice(dPos);
            if (d) toPos.GetPlan()->SetPosition(*d, toPos.GetSmartPos());
        }
    }
}

void ChipPlanFunc::DeviceHandle(PlanPos pos, int code)
{
    std::shared_ptr<Device> d = nullptr;
    if (pos.IsLocated())
    {
        d = GetDevice(pos);
    }
    else
    {
        d = pos.GetPlan()->GetHandle();
    }
    if (d) d->Handle(code);
}
void ChipPlanFunc::WireHandle(PlanPos pos1, PlanPos pos2, int code)
{
    if (true)//(MatchOnPlan(pos1, pos2))
    {
        auto w = GetWire(pos1, pos2);
        if (w) w->Handle(code);
    }
}

bool ChipPlanFunc::MatchOnPlan(PlanPos & pos1, PlanPos & pos2)
{
    PlanPos np1 { pos1.GetGrid() };
    PlanPos np2 { pos2.GetGrid() };

    if (pos1.IsLocated()) {
        np1.SetPosSmart( pos1.GetSmartPos() );
    }
    if (pos2.IsLocated()) {
        np2.SetPosSmart( pos2.GetSmartPos() );
    }
    
    std::shared_ptr<ChipHandle> ref1 = nullptr;
    if (not pos1.IsLocated()) ref1 = pos1.GetPlan()->GetHandle();
    std::shared_ptr<ChipHandle> ref2 = nullptr;
    if (not pos2.IsLocated()) ref2 = pos2.GetPlan()->GetHandle();
    if (pos1.GetPlan() == pos2.GetPlan())
    {
        //pos1 - pos2
    }
    else if (ref1 and ref1->GetContainer() == pos2.GetPlan())
    {
        //ref1 - pos2
        np1.SetGrid( ref1->GetContainer()->GetGrid() );
        np1.SetPosSmart( ref1->GetSmartPos() );
    }
    else if (ref2 and pos1.GetPlan() == ref2->GetContainer())
    {
        //pos1 - ref2
        np2.SetGrid( ref2->GetContainer()->GetGrid() );
        np2.SetPosSmart( ref2->GetSmartPos() );
    }
    else if (ref1 and ref2 and ref1->GetContainer() == ref2->GetContainer())
    {
        //ref1 - ref2
        np1.SetGrid( ref1->GetContainer()->GetGrid() );
        np1.SetPosSmart( ref1->GetSmartPos() );
        np2.SetGrid( ref2->GetContainer()->GetGrid() );
        np2.SetPosSmart( ref2->GetSmartPos() );
    }
    else
    {
        return false;
    }
    pos1 = np1;
    pos2 = np2;
    return true;
}

