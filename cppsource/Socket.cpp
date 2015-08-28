/* 
 * File:   Socket.cpp
 * Author: Dormouse
 * 
 * Created on 06 April 2015, 19:34
 */

#include "Socket.hpp"

const float RADIUS {20.f};
const sf::Vector2f MAIN_OFFSET { (GRID_SIZE/2.f) - sf::Vector2f(RADIUS, RADIUS) };

const sf::Vector2f WIRE_IN_OFFSET { MAIN_OFFSET + sf::Vector2f{ 0.f, RADIUS} };
const sf::Vector2f WIRE_OUT_OFFSET { MAIN_OFFSET + sf::Vector2f{RADIUS * 2.f, RADIUS} };

SocketView::SocketView(const Socket & socket_p)
    :DeviceView(socket_p.GetWorldPos()), shape( RADIUS )
{
}

void SocketView::Draw(sf::RenderTarget & rt, const Socket & s)
{
    UpdatePos(s.GetWorldPos());
    shape.setPosition( actualPos + MAIN_OFFSET );
    shape.setFillColor(sf::Color::Red);
    rt.draw(shape);
}





#include "Wire.hpp"

Socket::Socket(int serial, sf::Vector2i pos_p)
    :Device(serial, pos_p), v(*this)
{}

void Socket::ReceiveCharge(bool charge, int weight, int slot)
{
    PushCharge(charge, 0);
}

bool Socket::CanRegisterIn(int slot) const
{
    CleanWireVectors();
    return inWires.size() == 0 ? true : false;
}

sf::Vector2f Socket::GetWireAttachPos(WireAttachSide was) const
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

void Socket::Draw(sf::RenderTarget & rt) { v.Draw(rt, *this); }
void Socket::Handle(int code) {}


//Gonna try having ChipPlan inherit from Wirable and not bother with these classes...

//class BeginBar : public Device, public Wirable
//{
//public:
//    virtual bool CanRegisterIn(int slot) const { return false; }
//    virtual void ReceiveCharge(bool charge, int weight, int slot) { throw; }
//    void BeginCharge(bool charge, int slot);
//};
//void BeginBar::BeginCharge(bool charge, int slot)
//{
//    PushCharge(charge, slot);
//}
//
//class EndBar : public Device, public Wirable
//{
//public:
//    virtual bool CanRegisterOut(int slot) const { return false; }
//    virtual void ReceiveCharge(bool charge, int weight, int slot);
//};
//void EndBar::ReceiveCharge(bool charge, int weight, int slot)
//{
//    plan->EndCharge(charge, slot);
//}

