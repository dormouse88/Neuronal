/* 
 * File:   wire.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:09
 */

#include "Wire.hpp"
#include "Neuron.hpp" //to get its position

Wire::Wire(Neuron & from_p, Neuron & to_p)
    :from(from_p), to(to_p), weight(1), firing(false)
{
    from.RegisterOut(this);
    to.RegisterIn(this);
}

void Wire::PreDelete()
{
    from.DeRegister(this);
    to.DeRegister(this);
}

bool Wire::operator==(const Wire& rhs) const
{
    return this->GetFrom() == rhs.GetFrom() and this->GetTo() == rhs.GetTo() ;
}
