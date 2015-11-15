/* 
 * File:   ChipHandle.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 17:00
 */

#include "ChipHandle.hpp"
#include "miscUtil.hpp"

const sf::Vector2f RECTANGLE { 90.f, 65.f };
const sf::Vector2f MAIN_OFFSET {0.f, 0.f}; // { (GRID_SIZE - RECTANGLE)/2.f };
const sf::Vector2f PLANID_OFFSET { MAIN_OFFSET.x + 18.f, MAIN_OFFSET.y + 2.f };

const sf::Vector2f WIRE_IN_OFFSET { MAIN_OFFSET.x, MAIN_OFFSET.y + RECTANGLE.y/2.f };
const sf::Vector2f WIRE_OUT_OFFSET { MAIN_OFFSET.x + RECTANGLE.x, MAIN_OFFSET.y + RECTANGLE.y/2.f };


ChipHandle::ChipHandle(int serial_p, sf::Vector2i pos_p, std::shared_ptr<ChipPlan> cont)
    :Device(serial_p, pos_p, cont), DeviceView( GetWorldPos() ), exploded(false), shape( RECTANGLE )
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(3);
    planNumText.setFont(ViewResources::GetInstance().font);
    planNumText.setCharacterSize(30);
    planNumText.setColor(sf::Color::Black);
}


void ChipHandle::ReceiveCharge(bool charge, int weight, int slot)
{
    if (plan) plan->StepIn(charge, slot);
}

bool ChipHandle::IsSlotted(SlottedSide) const
{
    return true;
}

bool ChipHandle::CanRegisterIn(int slot) const
{
    CleanWireVectors();
    for (auto w: inWires) {
        if (w.lock()->GetToSlot() == slot) return false;
    }
    return true;
}
bool ChipHandle::CanRegisterOut(int slot) const
{
    CleanWireVectors();
    for (auto w: outWires) {
        if (w.lock()->GetFromSlot() == slot) return false;
    }
    return true;
}


void ChipHandle::StepOut(bool charge, int slot)
{
    PushCharge(charge, slot);
}

void ChipHandle::LogicAct()
{
    if (plan) plan->PassOnAct();
}
void ChipHandle::LogicCalculate()
{
    if (plan) plan->PassOnCalculate();
}


sf::Vector2f ChipHandle::GetWireAttachPos(WireAttachSide was) const
{
    sf::Vector2f wirePos;
    if (exploded)
    {
        RectWorld planBound { plan->GetWorldBound() };
        VectorWorld objectSize { planBound.width, planBound.height };
        if (was == WireAttachSide::IN) {
            wirePos = CalculateOffset(objectSize) + VectorWorld {objectSize.x *.0f, objectSize.y *.5f };
        }
        else {
            wirePos = CalculateOffset(objectSize) + VectorWorld {objectSize.x *1.f, objectSize.y *.5f };
        }
    }
    else
    {
        if (was == WireAttachSide::IN) {
            wirePos = CalculateOffset(RECTANGLE) + WIRE_IN_OFFSET;
        }
        else {
            wirePos = CalculateOffset(RECTANGLE) + WIRE_OUT_OFFSET;
        }
    }
    return wirePos;
}

void ChipHandle::Draw(sf::RenderTarget & rt)
{
    if (exploded)
    {
        //Refresh Plan Offset...
        plan->GetGrid()->SetOffset( VectorWorld{0.f, 0.f} );
        RectWorld innerPlanBound { plan->GetWorldBound() };
        VectorWorld innerPlanTopLeft { innerPlanBound.left, innerPlanBound.top };
        VectorWorld innerPlanSize {innerPlanBound.width, innerPlanBound.height};
        VectorWorld outerTopLeftPos = CalculateOffset(innerPlanSize);
        VectorWorld offsetFromZero = outerTopLeftPos - innerPlanTopLeft;
        plan->GetGrid()->SetOffset( offsetFromZero );
        
        plan->SubDraw(rt);
    }
    else
    {
        UpdatePos( CalculateOffset(RECTANGLE) );
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
    if (code == 1) {
        SetExploded(true);
    }
    if (code == 2) {
        SetExploded(false);
    }
}

VectorDumb ChipHandle::GetPlodedSize()
{
    if (exploded)
    {
        return VectorDumb{ plan->GetDumbBound().width, plan->GetDumbBound().height };
    }
    else return VectorDumb{1,1};
}

void ChipHandle::SetExploded(bool yes)
{
    if (not exploded and yes)
    {
        if (not plan->IsEmpty())
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
