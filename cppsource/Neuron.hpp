/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include "Device.hpp"

class Neuron : public Device
{
public:
    Neuron(sf::Vector2i pos_p);
    virtual ~Neuron() {;}

    virtual bool IsInstant() {return false;}
    virtual void ReceiveCharge(int weight);
    virtual void PushCharge();
    virtual void CalculateFiring();

    int GetThreshold() const {return threshold;}
//    void SetThreshold(int t) {threshold = t;}
    void ModifyThreshold(int v) {threshold += v;}
    bool GetFiring() const {return firing;}

private:
    bool firing;
    int threshold;
    int total;
};

#endif	/* NEURON_HPP */

