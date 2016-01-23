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


ChipHandle::ChipHandle(int serial_p, sf::Vector2i pos_p, PlanShp cont)
    :Device(serial_p, pos_p, cont)
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
void ChipHandle::Refresh(Tag slot)
{
    if (subPlan_) subPlan_->StepInRefresh(slot);
    //Something like this...
    //(has charge state saved in a map or something (inPorts))
    //
    //bool newState = false;
    //if ( inPorts.at(slot).GetTotalIncomingCharge() >= 1 )
    //    newState = true;
    //if (newState != inPorts.at(slot).charge) {
    //    inPorts.at(slot).charge = newState;
    //    PropagateRefresh(slot);
    //}
}

//(Called on right hand side of handle by other device)
bool ChipHandle::GetOutgoingCharge(Tag slot)
{
    if (subPlan_)
        return subPlan_->StepInGetOutgoingCharge(slot);
    else
        return false;
    //this perhaps?...
    //return outPorts.at(slot).charge;
}

VectorWorld ChipHandle::GetWireAttachPos(WireAttachSide was) const
{
    VectorWorld wirePos;
    if (exploded_)
    {
        RectWorld planBound { subPlan_->GetWorldPaddedBoundPlusPorts() };  //not sure
        VectorWorld objectSize { planBound.width, planBound.height };
        if (was == WireAttachSide::IN) {
            wirePos = CalculateOffsetForCentering(objectSize) + VectorWorld {objectSize.x *.0f, objectSize.y *.5f };
        }
        else {
            wirePos = CalculateOffsetForCentering(objectSize) + VectorWorld {objectSize.x *1.f, objectSize.y *.5f };
        }
    }
    else
    {
        if (was == WireAttachSide::IN) {
            wirePos = CalculateOffsetForCentering(RECTANGLE) + WIRE_IN_OFFSET;
        }
        else {
            wirePos = CalculateOffsetForCentering(RECTANGLE) + WIRE_OUT_OFFSET;
        }
    }
    return wirePos;
}

bool ChipHandle::IsSlotted(SlottedSide) const
{
    return true;
}
bool ChipHandle::CanRegisterWire(InOut side, Tag slot) const
{
    if (side == InOut::IN)
        return IsTagFree(InOut::IN, slot);
    else
        return IsTagFree(InOut::OUT, slot);
}




//Device...
void ChipHandle::InnerStep()
{
    if (subPlan_) subPlan_->PassOnAct();
}
void ChipHandle::PreInnerStep()
{
    if (subPlan_) subPlan_->PassOnCalculate();
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
void ChipHandle::StepOutRefresh(Tag slot)
{
    PropagateRefresh(slot);
}
//(called by plan onto left hand side of handle)
bool ChipHandle::StepOutGetOutgoingCharge(Tag slot)
{
    if (GetTotalIncomingWeight(slot) >= 1)
        return true;
    else
        return false;
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




