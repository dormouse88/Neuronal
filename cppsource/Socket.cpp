/* 
 * File:   Socket.cpp
 * Author: Dormouse
 * 
 * Created on 06 April 2015, 19:34
 */

#include "Socket.hpp"
#include "Wire.hpp"

Socket::Socket(int serial, sf::Vector2i pos_p)
    :Device(serial, pos_p)
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

