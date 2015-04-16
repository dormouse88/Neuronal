/* 
 * File:   wire.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:09
 */

#ifndef WIRE_HPP
#define	WIRE_HPP

#include "Gobject.hpp"
class Device;

class Wire : public Gobject
{
public:
    Wire(Device & from_p, Device & to_p);
    bool operator==(const Wire& rhs) const;

    void PushCharge();
    void ReceiveCharge(bool f);
    
    bool GetFiring() const {return firing;}
    int GetWeight() const {return weight;}
    void SetWeight(int w) {weight = w;}

    const Device& GetFrom() const {return from; }
    const Device& GetTo() const {return to; }
    
private:
    Device& from;
    Device& to;
    signed weight;
    bool firing;
};

#endif	/* WIRE_HPP */

