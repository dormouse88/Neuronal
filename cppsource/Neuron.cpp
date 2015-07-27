/* 
 * File:   Neuron.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:05
 */

#include "Neuron.hpp"
#include "Wire.hpp"

Neuron::Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p)
    :Device(serial_p, pos_p), firing(false), threshold(threshold_p), receivedCharge(0)
{}

void Neuron::ReceiveCharge(bool charge, int weight, int slot)
{
    if (charge) receivedCharge += weight;
}

void Neuron::LogicAct()
{
    PushCharge(GetFiring(), 0);
}

void Neuron::LogicCalculate()
{
    if (receivedCharge >= threshold) SetFiring(true);
    else SetFiring(false);
    receivedCharge = 0;
}

