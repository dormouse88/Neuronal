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
    void ModifyThreshold(int v) {threshold += v;}

private:
    int threshold;
    int total;
};

#endif	/* NEURON_HPP */

