/* 
 * File:   wire.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:09
 */

#include "Wire.hpp"
#include "Device.hpp" //to get its position

Wire::Wire(Wirable & from_p, Wirable & to_p, signed weight_p)
    :from(from_p), to(to_p), weight(weight_p), firing(false), fromSlot(0), toSlot(0)
{}

bool Wire::operator==(const Wire& rhs) const
{
    return this == &rhs;
//    return this->GetFrom() == rhs.GetFrom() and this->GetTo() == rhs.GetTo() ;
}

void Wire::ReceiveCharge(bool f)
{
    firing = f;
    to.ReceiveCharge(firing, weight, toSlot);
}