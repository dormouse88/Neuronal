/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include "PinDevice.hpp"

class Neuron : public PinDevice
{
public:
    Neuron(sf::Vector2i pos_p);
    virtual ~Neuron() {;}

    void StepPartOne();
    void StepPartTwo();
    
    int GetThreshold() const {return threshold;}
    void SetThreshold(int t) {threshold = t;}
    void ModifyThreshold(int v) {threshold += v;}
    bool GetFiring() const {return firing;}

private:
    bool firing;
    int threshold;
};

#endif	/* NEURON_HPP */

