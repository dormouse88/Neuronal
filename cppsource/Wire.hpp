/* 
 * File:   wire.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:09
 */

#ifndef WIRE_HPP
#define	WIRE_HPP

#include "Gobject.hpp"
class Neuron;

class Wire// : public Gobject
{
public:
    Wire(Neuron & from_p, Neuron & to_p);

    bool GetFiring() const {return firing;}
    void SetFiring(bool f) {firing = f;}
    int GetWeight() const {return weight;}
    void SetWeight(int w) {weight = w;}

    const Neuron& GetFrom() const {return from; }
    const Neuron& GetTo() const {return to; }
    
private:
    Neuron& from;
    Neuron& to;
    signed weight;
    bool firing;
};

#endif	/* WIRE_HPP */

