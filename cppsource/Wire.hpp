/* 
 * File:   wire.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:09
 */

#ifndef WIRE_HPP
#define	WIRE_HPP

#include "Gobject.hpp"
class PinDevice;

class Wire// : public Gobject
{
public:
    Wire(PinDevice & from_p, PinDevice & to_p);
    void PreDelete();
    bool operator==(const Wire& rhs) const;

    bool GetFiring() const {return firing;}
    void SetFiring(bool f) {firing = f;}
    int GetWeight() const {return weight;}
    void SetWeight(int w) {weight = w;}

    const PinDevice& GetFrom() const {return from; }
    const PinDevice& GetTo() const {return to; }
    
private:
    PinDevice& from;
    PinDevice& to;
    signed weight;
    bool firing;
};

#endif	/* WIRE_HPP */

