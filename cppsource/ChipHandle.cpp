/* 
 * File:   ChipHandle.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 17:00
 */

#include "ChipHandle.hpp"
#include "miscUtil.hpp"

const sf::Vector2f RECTANGLE { 90.f, 65.f };

const sf::Vector2f PLANID_OFFSET { 18.f, 2.f };

const sf::Vector2f WIRE_IN_OFFSET  { RECTANGLE.x * 0.f, RECTANGLE.y *.5f };
const sf::Vector2f WIRE_OUT_OFFSET { RECTANGLE.x * 1.f, RECTANGLE.y *.5f };


ChipHandle::ChipHandle(PlanID pid, VectorSmart pos, PlanShp cont)
    :Device(pid, pos, cont)
    , exploded_(false)
    , shape_( RECTANGLE )
{
    shape_.setOutlineColor(sf::Color::White);
    shape_.setOutlineThickness(3);
    planNumText_.setFont(ViewResources::GetInstance().font);
    planNumText_.setCharacterSize(30);
    planNumText_.setColor(sf::Color::Black);
}




//PlanOwned...
void ChipHandle::Draw(sf::RenderTarget & rt)
{
    if (exploded_)
    {
        //Refresh Plan Offset...
        subPlan_->GetGrid()->SetOffset( VectorWorld{0.f, 0.f} );
        RectWorld innerPlanBound { subPlan_->GetWorldPaddedBoundPlusPorts() };  //plus ports
        VectorWorld innerPlanTopLeft { innerPlanBound.left, innerPlanBound.top };
        VectorWorld innerPlanSize {innerPlanBound.width, innerPlanBound.height};
        VectorWorld outerTopLeftPos = CalculateOffsetForCentering(innerPlanSize);
        VectorWorld offsetFromZero = outerTopLeftPos - innerPlanTopLeft;
        subPlan_->GetGrid()->SetOffset( offsetFromZero );
        
        subPlan_->SubDraw(rt);
    }
    else
    {
        VectorWorld perceivedPos { CalculateOffsetForCentering(RECTANGLE) };
        shape_.setPosition( perceivedPos );
        shape_.setFillColor(sf::Color::Yellow);
        std::string text = patch::to_string( subPlan_->GetPlanID() );
        if (subPlan_->IsModified()) text.append("*");
        planNumText_.setString( text );
        planNumText_.setPosition( perceivedPos + PLANID_OFFSET );
        rt.draw(shape_);
        rt.draw(planNumText_);
    }
}

void ChipHandle::Handle(int code)
{
    ;
}





//Wirable...
void ChipHandle::StructuralRefresh()
{
    if (subPlan_)
    {
        subPlan_->ReCalculatePorts(ZoomSide::HEAD);
        subPlan_->ReCalculatePorts(ZoomSide::TAIL);
        subPlan_->RecalculateBounds();
    }
}
void ChipHandle::ReCalculateCharge(Tag slot)
{
    if (subPlan_)
        subPlan_->StepInReCalculateCharge(slot);
}

//(Called on right hand side of handle by other device)
Charge ChipHandle::GetOutgoingCharge(Tag tag)
{
    if (subPlan_)
        return subPlan_->StepInGetOutgoingCharge(tag);
    else
        return Charge::OFF;
}

VectorWorld ChipHandle::GetWireAttachPos(WireAttachSide was, Tag tag) const
{
    VectorWorld wirePos;
    if (exploded_)
    {
        PortLocation port;
        port.side = (was == WireAttachSide::IN) ? ZoomSide::HEAD : ZoomSide::TAIL ;
        port.num = subPlan_->MapTagToPort(port.side, tag);

//        PlanPos ppos { subPlan_->GetPortSmartPos(port), subPlan_->GetGrid() };
//        wirePos = ppos.GetWorldPos();
        VectorSmart cell = subPlan_->GetPortSmartPos(port);
        wirePos = subPlan_->GetGrid()->MapSmartToWorld( cell );
        wirePos.y += subPlan_->GetGrid()->WorldSizeOf( cell ).y * 0.5f;

        if (was == WireAttachSide::OUT)
            wirePos.x += subPlan_->GetGrid()->WorldSizeOf( cell ).x;
    }
    else
    {
        if (was == WireAttachSide::IN)
            wirePos = CalculateOffsetForCentering(RECTANGLE) + WIRE_IN_OFFSET;
        else
            wirePos = CalculateOffsetForCentering(RECTANGLE) + WIRE_OUT_OFFSET;
    }
    return wirePos;
}

bool ChipHandle::IsSlotted(SlottedSide) const
{
    return true;
}

Tag ChipHandle::GetFirstFreeTag(InOut side)
{
    if (subPlan_)
        return subPlan_->GetFirstFreeTag( (side==InOut::IN)? InOut::OUT : InOut::IN );
    return NULL_TAG;
}

bool ChipHandle::CanRegisterAnyWire(InOut side, Tag slot) const
{
    return true;
//    if (side == InOut::IN)
//        return IsTagFree(InOut::IN, slot);
//    else
//        return IsTagFree(InOut::OUT, slot);
}




//Device...
void ChipHandle::InnerStep()
{
    if (subPlan_) subPlan_->PassOnInnerStep();
}
void ChipHandle::PreInnerStep()
{
    if (subPlan_) subPlan_->PassOnPreInnerStep();
}
VectorDumb ChipHandle::GetPlodedSize()
{
    if (exploded_)
    {
        return VectorDumb{ subPlan_->GetDumbPaddedBound().width, subPlan_->GetDumbPaddedBound().height };
    }
    else return VectorDumb{1,1};
}






//RefererInterface...
//(called by plan onto right hand side of handle)
void ChipHandle::StepOutReCalculateCharge(Tag slot)
{
    PropagateRefresh(slot);
}
//(called by plan onto left hand side of handle)
Charge ChipHandle::StepOutGetOutgoingCharge(Tag slot)
{
    if (GetTotalIncomingWeight(slot) >= 1)
        return Charge::ON;
    else
        return Charge::OFF;
}
void ChipHandle::SetModified()
{
    auto cont = GetContainer();
    if (cont) cont->SetModified();
}
void ChipHandle::SetSubPlan(PlanShp p, std::shared_ptr<RefererInterface> myself)
{
    subPlan_ = p;
    p->RegisterReferer( myself );
    SetModified();
    p->PlodeRefreshOutwards();
}
PlanShp ChipHandle::GetSubPlan()
{
    return subPlan_;
}






//Self...
bool ChipHandle::IsExploded()
{
    return exploded_;
}

void ChipHandle::SetExploded(bool exploded)
{
    if (exploded_ != exploded)
    {
        exploded_ = exploded;
        GetContainer()->PlodeRefresh(GetSmartPos());
    }
}




