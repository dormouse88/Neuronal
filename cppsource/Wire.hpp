/* 
 * File:   wire.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:09
 */

#ifndef WIRE_HPP
#define	WIRE_HPP

#include "Gobject.hpp"
class Wirable;

class Wire : public Gobject
{
public:
    Wire(Wirable & from_p, Wirable & to_p, signed weight_p);
    bool operator==(const Wire& rhs) const;

    void ReceiveCharge(bool f);
    
    bool GetFiring() const {return firing;}
    int GetWeight() const {return weight;}
    void SetWeight(int w) {weight = w;}

    const Wirable& GetFrom() const {return from; }
    const int GetFromSlot() const {return fromSlot; }
    const Wirable& GetTo() const {return to; }
    const int GetToSlot() const {return toSlot; }
    
private:
    Wirable& from;
    int fromSlot;
    Wirable& to;
    int toSlot;
    signed weight;
    bool firing;
};

#endif	/* WIRE_HPP */

