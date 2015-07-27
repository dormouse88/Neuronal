/* 
 * File:   Neuron.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:05
 */

#ifndef NEURON_HPP
#define	NEURON_HPP

#include "Device.hpp"
#include "Wirable.hpp"

class Neuron : public Device
{
public:
    Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p);
    virtual ~Neuron() {;}

    virtual std::string SerialName() const { return "NEUR";}
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    virtual void LogicAct();
    virtual void LogicCalculate();

    bool GetFiring() const                      {return firing;}
    void SetFiring(bool f)                      {firing = f;}
    int GetThreshold() const {return threshold;}
    void ModifyThreshold(int v) {threshold += v;}

private:
    bool firing;
    int threshold;
    int receivedCharge;
};

#endif	/* NEURON_HPP */

