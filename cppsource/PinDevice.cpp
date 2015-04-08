/* 
 * File:   PinDevice.cpp
 * Author: Dormouse
 * 
 * Created on 06 April 2015, 19:09
 */

#include "PinDevice.hpp"
#include "Wire.hpp"

PinDevice::PinDevice(sf::Vector2i pos_p, bool multiInput_p)
    :pos(pos_p), multiInput(multiInput_p)
{}


void PinDevice::RegisterIn(Wire* wp)
{
    if (multiInput == false and inWires.size() >= 1)
    {
        inWires.emplace_back(wp);
    }
}

void PinDevice::RegisterOut(Wire* wp)
{
    outWires.emplace_back(wp);
}


void PinDevice::DeRegister(Wire* wp)
{
    auto DeReg = [&] (std::vector<Wire*> & container) {
        auto new_end = std::remove_if(std::begin(container), std::end(container), [&] (Wire* param) {return *wp == *param; } );
        container.erase(new_end, std::end(container));
    };
    DeReg(inWires);
    DeReg(outWires);
}
