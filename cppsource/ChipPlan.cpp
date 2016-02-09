/* 
 * File:   ChipPlan.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 16:57
 */

#include "ChipPlan.hpp"
#include "ChipHandle.hpp"
#include <cassert>
#include <algorithm>
#include <iterator>
#include "PlanGroupData.hpp" //fwd dec
#include "PlanPos.hpp"  //fwd dec
#include "WiringPair.hpp"  //fwd dec

const int PADDING { 2 };

ChipPlan::ChipPlan(std::shared_ptr<PlanGrid> g, std::shared_ptr<PlanGroupData> u)
    :Wirable(), planID(0), modified(false), planGrid(g), planGroupData_(u)
{
    RecalculateBounds();
}

void ChipPlan::RegisterReferer(std::shared_ptr<RefererInterface> ref)
{
    referer = ref;
}
std::shared_ptr<RefererInterface> ChipPlan::GetReferer()
{
    return referer;
}
HandleShp ChipPlan::GetHandle()
{
    return std::dynamic_pointer_cast<ChipHandle>( referer );
}



//Wirable...
void ChipPlan::StructuralRefresh()
{
    ReCalculatePorts(ZoomSide::HEAD);
    ReCalculatePorts(ZoomSide::TAIL);
    RecalculateBounds();
}
void ChipPlan::ReCalculateCharge(Tag slot)
{
    if (referer)
        referer->StepOutReCalculateCharge(slot);
}
Charge ChipPlan::GetOutgoingCharge(Tag slot)
{
    if (referer)
        return referer->StepOutGetOutgoingCharge(slot);
    return Charge::OFF;
}
VectorWorld ChipPlan::GetWireAttachPos(WireAttachSide was, Tag tag) const
{
    //Because ChipPlans are wired internally, the wires come OUT of the left...
    PortLocation port;
    port.side = (was == WireAttachSide::IN) ? ZoomSide::TAIL : ZoomSide::HEAD ;
    port.num = MapTagToPort(port.side, tag);
    VectorSmart cell = GetPortSmartPos(port);
    VectorWorld wirePos;
    wirePos = planGrid->MapSmartToWorld( cell );
    wirePos.y += planGrid->WorldSizeOf( cell ).y * 0.5f;
    if (was == WireAttachSide::OUT)
        wirePos.x += planGrid->WorldSizeOf( cell ).x;
    return wirePos;
}

Tag ChipPlan::GetFirstFreeTag(InOut was)
{
    const int AUTO_TAG_MAX_LENGTH = 20;
    const std::string AUTO_TAG_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    auto & ports = (was == InOut::OUT) ? inPorts_ : outPorts_ ;

    //Initialize tag to the first available value.
    Tag autoTag;
    for (int i = 0; i <= AUTO_TAG_MAX_LENGTH; ++i)
    {
        autoTag.push_back( AUTO_TAG_CHARS.front() );
        for (auto c: AUTO_TAG_CHARS)
        {
            autoTag.back() = c;
            if (ports.count(autoTag) == 0)
            {
                return autoTag;
            }
        }
        autoTag.back() = AUTO_TAG_CHARS.front();
    }
    throw "NAME MAX LENGTH Reached!"; // Highly unlikely to reach this point with Max of 30, but still bad form.;
}

bool ChipPlan::CanRegisterAnyWire(InOut side, Tag slot) const
{
    return true;
//    if (side == InOut::IN)
//        return IsTagFree(InOut::IN, slot);
//    else
//        return IsTagFree(InOut::OUT, slot);
}



//"Referred"...
void ChipPlan::StepInReCalculateCharge(Tag slot)
{
    PropagateRefresh(slot);
}
Charge ChipPlan::StepInGetOutgoingCharge(Tag slot)
{
    if ( GetTotalIncomingWeight(slot) >= 1 )
        return Charge::ON;
    else
        return Charge::OFF;
}
void ChipPlan::PassOnInnerStep()
{
    for (auto & d: devices) d->InnerStep();
}
void ChipPlan::PassOnPreInnerStep()
{
    for (auto & d: devices) d->PreInnerStep();
}






/////////////////////////////////////////

void ChipPlan::SetPosition(DeviceShp d, VectorSmart newPos)
{
    if (IsPositionFree(newPos))
    {
        auto oldPos = d->GetSmartPos();
        d->SetPosSmart( newPos );
        SetModified();
        PlodeRefresh(oldPos);
        PlodeRefresh(newPos);
    }
}
PlanID ChipPlan::GetFreeSerial() const
{
    for (int i = 1; true; i++)
    {
        if (IsSerialFree(i)) return i;
    }
}
bool ChipPlan::IsSerialFree(PlanID pid) const
{
    for (const auto & x : devices) {
        if (x->GetPlanID() == pid) return false;
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

void ChipPlan::ImportDevice(DeviceShp device)
{
    devices.emplace_back(device);
    SetModified();
}
void ChipPlan::ImportWire(WireShp wire)
{
    wires.emplace_back(wire);
    const_cast<Wirable&>(wire->GetFrom()).StructuralRefresh();
    const_cast<Wirable&>(wire->GetTo()).StructuralRefresh();
    SetModified();
}
void ChipPlan::RemoveDevice(PlanPos pos)
{
    DeviceShp device = pos.GetDevice();
    HandleShp h = pos.GetDeviceAsHandle();
    if ( device and (not h or h->GetSubPlan()->IsEmpty()) )
    {
        for (auto w : GetWires(device, true, true))
        {
            RemoveWire(w);
            //w->Zingaya();
        }
        device->Zingaya();
        CleanVectors();
        SetModified();
    }
}
void ChipPlan::RemoveWire(WireShp wire)
{
    if (wire)
    {
        Wirable & fromRef = const_cast<Wirable&>(wire->GetFrom());
        Wirable & toRef = const_cast<Wirable&>(wire->GetTo());
        wire->Zingaya();
        fromRef.StructuralRefresh();
        toRef.StructuralRefresh();
        CleanVectors();
        SetModified();
    }
}
void ChipPlan::CleanVectors()
{
    {
        auto remove_func = [] (DeviceShp eachDevice) {return eachDevice->IsDead();};
        auto new_end = std::remove_if(std::begin(devices), std::end(devices), remove_func );
        devices.erase(new_end, std::end(devices) );
    }
    {
        auto remove_func = [] (WireShp eachWire) {return eachWire->IsDead();};
        auto new_end = std::remove_if(std::begin(wires), std::end(wires), remove_func);
        wires.erase(new_end, std::end(wires) );
    }
}

void ChipPlan::SetModified()
{
    StructuralRefresh();

    if (not modified)
    {
        if (referer)
            referer->SetModified();
        modified = true;
    }
}


PlanRegion ChipPlan::GetRegion(VectorSmart pos)
{
    if (pos.x == box_tl_.x or pos.x == box_br_.x)
    {
        if (pos.y == box_tl_.y or pos.y == box_br_.y)
            return PlanRegion::WHOLEPLAN;
        else
            return PlanRegion::PORTS;
    }
    else if (pos.x > box_tl_.x and pos.x < box_br_.x and pos.y >= box_tl_.y and pos.y <= box_br_.y)
        return PlanRegion::CONTENTS;
    else if ( not (pos.y >= box_tl_.y and pos.y <= box_br_.y and pos.x >= box_tl_.x and pos.x <= box_br_.x) )
        return PlanRegion::NONE;
    assert(false);
}

PortLocation ChipPlan::GetPort(VectorSmart pos)
{
    assert(GetRegion(pos) == PlanRegion::PORTS);
    PortLocation ret;

    if (pos.x == box_tl_.x)
        ret.side = ZoomSide::HEAD;
    else if (pos.x == box_br_.x)
        ret.side = ZoomSide::TAIL;
    else
        assert(false);
    
    ret.num = pos.y - box_tl_.y;
    if ( ret.num < 1 or (ret.side == ZoomSide::HEAD and ret.num > inPorts_.size()) or (ret.side == ZoomSide::TAIL and ret.num > outPorts_.size()) )
        ret.num = 0;

    return ret;
}


DeviceShp ChipPlan::GetDevice(VectorSmart pos)
{
    for (auto & x: devices) {
        if (pos == x->GetSmartPos()) {
            return x;
        }
    }
    return nullptr;
}
DeviceShp ChipPlan::GetDevice(PlanID pid)
{
    for (auto & x: devices) {
        if (pid == x->GetPlanID()) {
            return x;
        }
    }
    return nullptr;
}

//WireShp ChipPlan::GetWire(WirableShp from, WirableShp to)
//{
//    for (auto & x: wires) {
//        if (from.get() == &x->GetFrom() and to.get() == &x->GetTo()) {
//            return x;
//        }
//    }
//    return nullptr;
//}

WireShp ChipPlan::GetWire(Shp<WiringPair> wp)
{
    for (auto & x: wires)
    {
        if ( wp->from.get() == &x->GetFrom() and wp->to.get() == &x->GetTo() and wp->fromTag == x->GetFromTag() and wp->toTag == x->GetToTag() )
            return x;
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
std::vector<WireShp > ChipPlan::GetWires(std::shared_ptr<Wirable> wirable, bool from, bool to)
{
    std::vector<WireShp > ret_vec;
    for (auto & x: wires) {
        if ((from and wirable.get() == &x->GetFrom()) or (to and wirable.get() == &x->GetTo())) {
            ret_vec.emplace_back(x);
        }
    }
    return ret_vec;
}


void ChipPlan::RecalculateBounds()
{
    //Calculate inner (contents) bound...
    VectorSmart tl;
    VectorSmart br;
    if (devices.size() > 0)
    {
        //set initial values
        VectorSmart firstPos = devices.at(0)->GetSmartPos();
        tl = firstPos;
        br = firstPos;
        for (auto & x: devices)
        {
            VectorSmart p = x->GetSmartPos();
            if (p.x < tl.x) tl.x = p.x;
            if (p.x > br.x) br.x = p.x;
            if (p.y < tl.y) tl.y = p.y;
            if (p.y > br.y) br.y = p.y;
        }
    }

    //Calculate outer (box) bound...
    VectorSmart o_tl = tl;
    VectorSmart o_br = br;
    {
        o_tl -= VectorSmart { PADDING, PADDING };
        o_br += VectorSmart { PADDING, PADDING };
        int portsNeeded = std::max(inPorts_.size(), outPorts_.size());
        int portOverflow = (portsNeeded + 2) - (o_br.y - o_tl.y + 1);  //2 is for the corners, 1 is because br is inclusive.
        portOverflow += 1; //to ensure there is always a "null" port
        if (portOverflow > 0)
            o_br.y += portOverflow;
    }
    
    //Set member variables and refresh if anything has changed...
    if (not (tl == inner_tl_ and br == inner_br_ and o_tl == box_tl_ and o_br == box_br_) )
    {
        inner_tl_ = tl;
        inner_br_ = br;
        box_tl_ = o_tl;
        box_br_ = o_br;
        PlodeRefreshOutwards();
    }
}

PlanRect ChipPlan::GetSmartInnerBound() const
{
    PlanRect b { PlanPos{ inner_tl_, planGrid }, PlanPos{ inner_br_, planGrid } };
    return b;
}
PlanRect ChipPlan::GetSmartPaddedBound() const
{
    assert(planGrid);
    PlanPos temp1 { box_tl_, planGrid };
    PlanPos temp2 { box_br_, planGrid };
    return PlanRect {temp1, temp2};
    PlanRect b { PlanPos{ box_tl_, planGrid }, PlanPos{ box_br_, planGrid } };
    return b;
}
RectDumb ChipPlan::GetDumbPaddedBound() const
{
    auto pr = GetSmartPaddedBound();
    return pr.GetRectDumb();
}
RectWorld ChipPlan::GetWorldPaddedBound() const
{
    assert(false);
    auto pr = GetSmartPaddedBound();
    return pr.GetRectWorld();
}
RectWorld ChipPlan::GetWorldPaddedBoundBox() const
{
    PlanRect pr = GetSmartPaddedBound();
    RectWorld rw = pr.GetRectWorld();
    rw.left += pr.tl.GetWorldSizeOf().x * 0.5f;
    rw.width -= pr.br.GetWorldSizeOf().x * 1.0f;
    return rw;
}
RectWorld ChipPlan::GetWorldPaddedBoundPlusPorts() const
{
    auto pr = GetSmartPaddedBound();
    return pr.GetRectWorld();
}
RectWorld ChipPlan::GetWorldPaddedBoundMinusPorts() const
{
    PlanRect pr = GetSmartPaddedBound();
    RectWorld rw = pr.GetRectWorld();
    rw.left += pr.tl.GetWorldSizeOf().x * 1.f;
    rw.width -= pr.br.GetWorldSizeOf().x * 2.0f;
    return rw;
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
    PlodeRefreshOutwards();
}

void ChipPlan::PlodeRefreshOutwards()
{
    //Plode refresh recursively back to base plan...
    auto refLock = GetHandle();
    if (refLock) {
        auto cont = refLock->GetContainer();
        cont->PlodeRefresh( refLock->GetSmartPos() );
    }
}





void ChipPlan::Draw(sf::RenderTarget & rt)
{
    SubDraw(rt);
}
void ChipPlan::SubDraw(sf::RenderTarget & rt)
{
    DrawBox(rt);
    DrawGridLines(rt);
    DrawTitle(rt);
    DrawParts(rt);
}

void ChipPlan::DrawBox(sf::RenderTarget & rt)
{
    sf::Color randCol {
         static_cast<sf::Uint8>( ((GetPlanID()+ 130) * 20000) % 255 )
        ,static_cast<sf::Uint8>( ((GetPlanID()+ 130) * 22000) % 255 )
        ,static_cast<sf::Uint8>( ((GetPlanID()+ 130) * 25000) % 255 )
    };
    if (IsEmpty()) randCol = sf::Color::Black;
    auto pB = GetWorldPaddedBoundBox(); //box
    sf::RectangleShape planBox;
    planBox.setFillColor( randCol * sf::Color{70,70,70} );
    planBox.setOutlineColor( sf::Color{95,95,95} );
    planBox.setOutlineThickness(-4.f);
    planBox.setPosition( sf::Vector2f{pB.left, pB.top} );
    planBox.setSize( sf::Vector2f{pB.width, pB.height} );
    rt.draw(planBox);
    
    //Grabber
    const sf::Vector2f GRABBER_SIZE { GRID_SIZE * 1.0f };
    //VectorWorld bPos = planBox.getPosition();
    sf::RectangleShape cornerBox;
    cornerBox.setFillColor( sf::Color{95,95,95} );
    cornerBox.setOrigin(GRABBER_SIZE.x * 0.5f, 0.f);
    cornerBox.setSize( GRABBER_SIZE );
    cornerBox.setPosition( pB.left, pB.top );
    rt.draw(cornerBox);
    cornerBox.setPosition( pB.left + pB.width, pB.top );
    rt.draw(cornerBox);
    cornerBox.setPosition( pB.left, pB.top + pB.height - GRABBER_SIZE.y );
    rt.draw(cornerBox);
    cornerBox.setPosition( pB.left + pB.width, pB.top + pB.height - GRABBER_SIZE.y );
    rt.draw(cornerBox);
}
void ChipPlan::DrawTitle(sf::RenderTarget & rt)
{
    auto pB = GetWorldPaddedBoundMinusPorts();  //unimportant
    sf::Text planNumText;
    planNumText.setFont( ViewResources::GetInstance().font );
    std::string textString { patch::to_string(GetPlanID()) };
    if (planGroupData_) {
        textString.append(" -- " + planGroupData_->GetNameByID(GetPlanID()));
        auto rels = planGroupData_->GetRelatives(GetPlanID());
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
    auto pB = GetWorldPaddedBoundBox();  //box i guess
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
    
    sf::ConvexShape shape;
    shape.setPointCount(8);
    const float BIG_H = GRID_SIZE.y * 0.5f;
    const float SMALL_H = GRID_SIZE.y * 0.35f;
    const float BIG_W = GRID_SIZE.x * 0.5f;
    const float SMALL_W = GRID_SIZE.x * 0.35f;
    shape.setPoint(0, sf::Vector2f{ 0,          BIG_H } );
    shape.setPoint(1, sf::Vector2f{ SMALL_W,    SMALL_H } );
    shape.setPoint(2, sf::Vector2f{ BIG_W,      0 } );
    shape.setPoint(3, sf::Vector2f{ SMALL_W,    -SMALL_H } );
    shape.setPoint(4, sf::Vector2f{ 0,          -BIG_H } );
    shape.setPoint(5, sf::Vector2f{ -SMALL_W,   -SMALL_H } );
    shape.setPoint(6, sf::Vector2f{ -BIG_W,     0 } );
    shape.setPoint(7, sf::Vector2f{ -SMALL_W,   SMALL_H } );
    shape.setFillColor( sf::Color { 150,0,150 } );
    shape.setOutlineColor( sf::Color{95,95,95} );
    shape.setOutlineThickness(-4.f);
    sf::Text text;
    text.setFont( ViewResources::GetInstance().font );
    text.setColor( sf::Color::Cyan );
    
    for (auto & p: inPorts_)
    {
        VectorSmart s_pos = GetPortSmartPos( PortLocation{ZoomSide::HEAD, p.second} );
        VectorWorld pos = planGrid->MapSmartToWorld(s_pos) + (planGrid->WorldSizeOf(s_pos) * 0.5f);
        shape.setPosition( pos );
        text.setPosition( pos );
        Tag tag = p.first;
        text.setString( patch::to_string(tag) );
        rt.draw(shape);
        rt.draw(text);
    }
    for (auto & p: outPorts_)
    {
        VectorSmart s_pos = GetPortSmartPos( PortLocation{ZoomSide::TAIL, p.second} );
        VectorWorld pos = planGrid->MapSmartToWorld(s_pos) + (planGrid->WorldSizeOf(s_pos) * 0.5f);
        shape.setPosition( pos );
        text.setPosition( pos );
        Tag tag = p.first;
        text.setString( patch::to_string(tag) );
        rt.draw(shape);
        rt.draw(text);
    }
}



bool ChipPlan::HasPort(PortLocation port) const
{
    if ( (port.side == ZoomSide::HEAD and port.num <= inPorts_.size()) or (port.side == ZoomSide::TAIL and port.num <= outPorts_.size()) )
        return true;
    else
        return false;
}

VectorSmart ChipPlan::GetPortSmartPos(PortLocation port) const
{
    if (port.num == 0)
        port.num = (port.side == ZoomSide::HEAD) ? inPorts_.size() +1 : outPorts_.size() +1 ;
    PlanRect r = GetSmartPaddedBound();
    VectorSmart start;
    start.x = (port.side == ZoomSide::HEAD) ? r.tl.GetSmartPos().x : start.x = r.br.GetSmartPos().x ;
    start.y = r.tl.GetSmartPos().y;
    return { start.x, start.y + port.num };
}

void ChipPlan::ReCalculatePorts(ZoomSide side)
{
    std::set<Tag> allTags;
    {
        std::set<Tag> inSet;
        std::set<Tag> outSet;
        if (side == ZoomSide::HEAD)
        {
            auto h = GetHandle();
            if (h)
                inSet  = h->GetTagCloud(InOut::IN);
            outSet = GetTagCloud(InOut::OUT);
        }
        else
        {
            inSet  = GetTagCloud(InOut::IN);
            auto h = GetHandle();
            if (h)
                outSet = h->GetTagCloud(InOut::OUT);
        }
        std::set_union(
            inSet.begin(), inSet.end(),
            outSet.begin(), outSet.end(),
            std::inserter(allTags, allTags.begin())
        );
    }
    auto & ports = (side == ZoomSide::HEAD) ? inPorts_ : outPorts_ ;
    ports.clear();
    PortNum num = 1;
    for (auto & x: allTags)
    {
        ports[x] = num;
        num++;
    }
}

PortNum ChipPlan::MapTagToPort(ZoomSide pSide, Tag tag) const
{
    const auto & ports = (pSide == ZoomSide::HEAD) ? inPorts_ : outPorts_ ;
    for (auto x: ports)
    {
        if (x.first == tag)
            return x.second;
    }
    return 0;
}

Tag ChipPlan::MapPortToTag(ZoomSide pSide, PortNum portNum) const
{
    const auto & ports = (pSide == ZoomSide::HEAD) ? inPorts_ : outPorts_ ;
    for (auto x: ports)
    {
        if (x.second == portNum)
            return x.first;
    }
    return NULL_TAG;
}

//{
//    auto & ports = (side == ZoomSide::HEAD) ? inPorts_ : outPorts_ ;
//    Tag ret = -1;
//    bool invalidated;
//    for (int i = 1; i<=SLOT_MAX; i++)
//    {
//        invalidated = false;
//        for (auto p: ports)
//        {
//            if (p.first == i)
//            {
//                invalidated = true;
//                break;
//            }
//        }
//        if (not invalidated)
//        {
//            ret = i;
//            break;
//        }
//    }
//    return ret;
//}

























//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////namespace

//DeviceShp ChipPlanFunc::GetDevice(PlanPos pos)
//{
//    return pos.GetPlan()->GetDevice(pos.GetSmartPos());
//}
//std::shared_ptr<Wirable> ChipPlanFunc::GetWirable(PlanPos pos)
//{
//    if (pos.IsLocated()) return GetDevice(pos);
//    else return pos.GetPlan();
//}


//
//WireShp ChipPlanFunc::GetWire(PlanPos pos1, PlanPos pos2)
//{
//    if (MatchOnPlan(pos1, pos2))  //(pos1.GetGrid() == pos2.GetGrid())
//    {
//        auto w1 = GetWirable(pos1);
//        auto w2 = GetWirable(pos2);
//        if (w1 and w2) return pos1.GetPlan()->GetWire(*w1, *w2);
//    }
//    return nullptr;
//}
//std::vector<WireShp > ChipPlanFunc::GetWires(PlanPos pos, bool from, bool to)
//{
//    auto w1 = GetWirable(pos);
//    if (w1) return pos.GetPlan()->GetWires(w1, from, to);
//    return std::vector<WireShp >();
//}
//
//void ChipPlanFunc::SetPosition(PlanPos dPos, PlanPos toPos)
//{
//    if ( MatchOnPlan(dPos, toPos) )
//    {
//        if ( dPos.IsLocated() and toPos.IsLocated() )
//        {
//            auto d = GetDevice(dPos);
//            if (d) toPos.GetPlan()->SetPosition(*d, toPos.GetSmartPos());
//        }
//    }
//}
//
//void ChipPlanFunc::DeviceHandle(PlanPos pos, int code)
//{
//    DeviceShp d = nullptr;
//    if (pos.IsLocated())
//    {
//        d = GetDevice(pos);
//    }
//    else
//    {
//        d = pos.GetPlan()->GetHandle();
//    }
//    if (d) d->Handle(code);
//}
//void ChipPlanFunc::WireHandle(PlanPos pos1, PlanPos pos2, int code)
//{
//    if (true)//(MatchOnPlan(pos1, pos2))
//    {
//        auto w = GetWire(pos1, pos2);
//        if (w) w->Handle(code);
//    }
//}

//bool ChipPlanFunc::MatchOnPlan(PlanPos & pos1, PlanPos & pos2)
//{
//    PlanPos np1 { pos1.GetGrid() };
//    PlanPos np2 { pos2.GetGrid() };
//
//    if (pos1.IsLocated()) {
//        np1.SetPosSmart( pos1.GetSmartPos() );
//    }
//    if (pos2.IsLocated()) {
//        np2.SetPosSmart( pos2.GetSmartPos() );
//    }
//    
//    HandleShp ref1 = nullptr;
//    if (not pos1.IsLocated()) ref1 = pos1.GetPlan()->GetHandle();
//    HandleShp ref2 = nullptr;
//    if (not pos2.IsLocated()) ref2 = pos2.GetPlan()->GetHandle();
//    if (pos1.GetPlan() == pos2.GetPlan())
//    {
//        //pos1 - pos2
//    }
//    else if (ref1 and ref1->GetContainer() == pos2.GetPlan())
//    {
//        //ref1 - pos2
//        np1.SetGrid( ref1->GetContainer()->GetGrid() );
//        np1.SetPosSmart( ref1->GetSmartPos() );
//    }
//    else if (ref2 and pos1.GetPlan() == ref2->GetContainer())
//    {
//        //pos1 - ref2
//        np2.SetGrid( ref2->GetContainer()->GetGrid() );
//        np2.SetPosSmart( ref2->GetSmartPos() );
//    }
//    else if (ref1 and ref2 and ref1->GetContainer() == ref2->GetContainer())
//    {
//        //ref1 - ref2
//        np1.SetGrid( ref1->GetContainer()->GetGrid() );
//        np1.SetPosSmart( ref1->GetSmartPos() );
//        np2.SetGrid( ref2->GetContainer()->GetGrid() );
//        np2.SetPosSmart( ref2->GetSmartPos() );
//    }
//    else
//    {
//        return false;
//    }
//    pos1 = np1;
//    pos2 = np2;
//    return true;
//}

