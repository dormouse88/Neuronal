/* 
 * File:   Socket.cpp
 * Author: Dormouse
 * 
 * Created on 06 April 2015, 19:34
 */

#include "Socket.hpp"
#include "Wire.hpp"

Socket::Socket(sf::Vector2i pos_p)
    :Device(pos_p, 1)
{}


void Socket::ReceiveCharge(int weight)
{
    SetFiring(weight);
    PushCharge();
}

void Socket::PushCharge()
{
    for (auto & w: outWires)
    {
        if (auto notDead = w.lock())
            notDead->ReceiveCharge(GetFiring());
    }
}
