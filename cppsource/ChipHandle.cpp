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


ChipHandle::ChipHandle(int serial_p, sf::Vector2i pos_p, std::shared_ptr<ChipPlan> cont)
    :Device(serial_p, pos_p, cont)
    , exploded(false)
    , shape( RECTANGLE )
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(3);
    planNumText.setFont(ViewResources::GetInstance().font);
    planNumText.setCharacterSize(30);
    planNumText.setColor(sf::Color::Black);
}




//PlanOwned...
void ChipHandle::Draw(sf::RenderTarget & rt)
{
    if (exploded)
    {
        //Refresh Plan Offset...
        plan->GetGrid()->SetOffset( VectorWorld{0.f, 0.f} );
        RectWorld innerPlanBound { plan->GetWorldPaddedBoundPlusPorts() };  //plus ports
        VectorWorld innerPlanTopLeft { innerPlanBound.left, innerPlanBound.top };
        VectorWorld innerPlanSize {innerPlanBound.width, innerPlanBound.height};
        VectorWorld outerTopLeftPos = CalculateOffsetForCentering(innerPlanSize);
        VectorWorld offsetFromZero = outerTopLeftPos - innerPlanTopLeft;
        plan->GetGrid()->SetOffset( offsetFromZero );
        
        plan->SubDraw(rt);
    }
    else
    {
        VectorWorld perceivedPos { CalculateOffsetForCentering(RECTANGLE) };
        shape.setPosition( perceivedPos );
        shape.setFillColor(sf::Color::Yellow);
        std::string text = patch::to_string( plan->GetPlanID() );
        if (plan->IsModified()) text.append("*");
        planNumText.setString( text );
        planNumText.setPosition( perceivedPos + PLANID_OFFSET );
        rt.draw(shape);
        rt.draw(planNumText);
    }
}

void ChipHandle::Handle(int code)
{
    if (code == 3) {
        if (IsExploded()) SetExploded(false);
        else SetExploded(true);
    }
    if (code == 4) {
        SetExploded(false);
    }
}





//Wirable...
void ChipHandle::Refresh(int slot)
{
    if (plan) plan->StepInRefresh(slot);
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
bool ChipHandle::GetOutgoingCharge(int slot)
{
    if (plan)
        return plan->StepInGetOutgoingCharge(slot);
    else
        return false;
    //this perhaps?...
    //return outPorts.at(slot).charge;
}

VectorWorld ChipHandle::GetWireAttachPos(WireAttachSide was) const
{
    VectorWorld wirePos;
    if (exploded)
    {
        RectWorld planBound { plan->GetWorldPaddedBoundPlusPorts() };  //not sure
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
bool ChipHandle::CanRegisterIn(int slot) const
{
    return IsInSlotFree(slot);
}
bool ChipHandle::CanRegisterOut(int slot) const
{
    return IsOutSlotFree(slot);
}






//Device...
void ChipHandle::InnerStep()
{
    if (plan) plan->PassOnAct();
}
void ChipHandle::PreInnerStep()
{
    if (plan) plan->PassOnCalculate();
}
VectorDumb ChipHandle::GetPlodedSize()
{
    if (exploded)
    {
        return VectorDumb{ plan->GetDumbPaddedBound().width, plan->GetDumbPaddedBound().height };
    }
    else return VectorDumb{1,1};
}






//RefererInterface...
//(called by plan onto right hand side of handle)
void ChipHandle::StepOutRefresh(int slot)
{
    PropagateRefresh(slot);
}
//(called by plan onto left hand side of handle)
bool ChipHandle::StepOutGetOutgoingCharge(int slot)
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
void ChipHandle::SetSubPlan(std::shared_ptr<ChipPlan> p, std::shared_ptr<RefererInterface> myself)
{
    plan = p;
    p->RegisterReferer( myself );
    SetModified();
    p->PlodeRefreshOutwards();
}
std::shared_ptr<ChipPlan> ChipHandle::GetSubPlan()
{
    return plan;
}






//Self...
bool ChipHandle::IsExploded()
{
    return exploded;
}

void ChipHandle::SetExploded(bool yes)
{
    if (not exploded and yes)
    {
        if (true)//not plan->IsEmpty())
        {
            exploded = true;
            GetContainer()->PlodeRefresh(GetSmartPos());
        }
    }
    else if (exploded and not yes)
    {
        exploded = false;
        GetContainer()->PlodeRefresh(GetSmartPos());
    }
}




