/* 
 * File:   ChipHandle.cpp
 * Author: Dormouse
 * 
 * Created on 17 June 2015, 17:00
 */

#include "ChipHandle.hpp"

const sf::Vector2f RECTANGLE { 90.f, 65.f };
const sf::Vector2f MAIN_OFFSET { (GRID_SIZE - RECTANGLE)/2.f };
const sf::Vector2f THRESHOLD_OFFSET { MAIN_OFFSET.x + 18.f, MAIN_OFFSET.y + 2.f };

const sf::Vector2f WIRE_IN_OFFSET { MAIN_OFFSET };
const sf::Vector2f WIRE_OUT_OFFSET { MAIN_OFFSET + RECTANGLE };


ChipHandle::ChipHandle(int serial_p, sf::Vector2i pos_p)
    :Device(serial_p, pos_p), DeviceView( GetWorldPos() ), shape( RECTANGLE )
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(3);
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
    if (was == WireAttachSide::IN) {
        wirePos = GetWorldPos() + WIRE_IN_OFFSET;
    }
    else {
        wirePos = GetWorldPos() + WIRE_OUT_OFFSET;
    }
    return wirePos;
}

void ChipHandle::Draw(sf::RenderTarget & rt)
{
    UpdatePos(GetWorldPos());
    shape.setPosition( actualPos + MAIN_OFFSET );
    shape.setFillColor(sf::Color::Yellow);
    rt.draw(shape);
}

void ChipHandle::Handle(int code)
{
    if (code == 1) {
        ;
    }
}
