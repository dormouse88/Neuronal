/* 
 * File:   wire.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:09
 */

#include "Wire.hpp"
#include "Device.hpp" //to get its position

Wire::Wire(Device & from_p, Device & to_p, signed weight_p)
    :from(from_p), to(to_p), weight(weight_p), firing(false)
{}

bool Wire::operator==(const Wire& rhs) const
{
    return this == &rhs;
//    return this->GetFrom() == rhs.GetFrom() and this->GetTo() == rhs.GetTo() ;
}

void Wire::PushCharge()
{
    to.ReceiveCharge(weight * firing);
}

void Wire::ReceiveCharge(bool f)
{
    firing = f;
    if (to.IsInstant()) PushCharge();
}