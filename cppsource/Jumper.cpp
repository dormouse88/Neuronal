/* 
 * File:   Jumper.cpp
 * Author: Dormouse
 * 
 * Created on 06 April 2015, 19:34
 */

#include "Jumper.hpp"

const float RADIUS {9.f};
const sf::Vector2f RECTANGLE { RADIUS * 2, RADIUS * 2 };

const sf::Vector2f WIRE_IN_OFFSET { sf::Vector2f{ 0.f, RADIUS} };
const sf::Vector2f WIRE_OUT_OFFSET { sf::Vector2f{RADIUS * 2.f, RADIUS} };

JumperView::JumperView(const Jumper & jumper_p)
    :DeviceView(jumper_p.GetWorldPos()), shape( RADIUS )
{
    shape.setFillColor( sf::Color{0,170,0} );
    shape.setOutlineColor( sf::Color{255,255,255} );
    shape.setOutlineThickness(2.f);
}

void JumperView::Draw(sf::RenderTarget & rt, const Jumper & s)
{
    UpdatePos(s.CalculateOffset(RECTANGLE));
    shape.setPosition( perceivedPos );
    rt.draw(shape);
}





#include "Wire.hpp"

Jumper::Jumper(int serial, sf::Vector2i pos_p, std::shared_ptr<ChipPlan> cont)
    :Device(serial, pos_p, cont), v(*this)
{}

void Jumper::ReceiveCharge(bool charge, int weight, int slot)
{
    PushCharge(charge, 0);
}

bool Jumper::CanRegisterIn(int slot) const
{
    CleanWireVectors();
    return inWires.size() == 0 ? true : false;
}

sf::Vector2f Jumper::GetWireAttachPos(WireAttachSide was) const
{
    sf::Vector2f wirePos;
    if (was == WireAttachSide::IN) {
        wirePos = CalculateOffset(RECTANGLE) + WIRE_IN_OFFSET;
    }
    else {
        wirePos = CalculateOffset(RECTANGLE) + WIRE_OUT_OFFSET;
    }
    return wirePos;
}

void Jumper::Draw(sf::RenderTarget & rt) { v.Draw(rt, *this); }
void Jumper::Handle(int code) {}


