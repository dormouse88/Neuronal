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
    Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p);
    virtual ~Neuron() {;}

    virtual std::string SerialName() const { return "NEUR";}
    virtual bool IsInstant() {return false;}
    virtual void ReceiveCharge(int weight);
    virtual void PushCharge();
    virtual void CalculateFiring();

    int GetThreshold() const {return threshold;}
    void ModifyThreshold(int v) {threshold += v;}

private:
    int threshold;
    int receivedCharge;
};

#endif	/* NEURON_HPP */

