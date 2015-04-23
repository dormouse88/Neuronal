/* 
 * File:   Neuron.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:05
 */

#include "Neuron.hpp"
#include "Wire.hpp"

const int INITIAL_THRESHOLD = 1;

Neuron::Neuron(sf::Vector2i pos_p)
    :Device(pos_p), threshold(INITIAL_THRESHOLD), total(0)
{}

void Neuron::ReceiveCharge(int weight)
{
    total += weight;
}

void Neuron::PushCharge()
{
    for (auto & w: outWires)
    {
        if (auto notDead = w.lock())
            notDead->ReceiveCharge(GetFiring());
    }
}

void Neuron::CalculateFiring()
{
    if (total >= threshold) SetFiring(true);
    else SetFiring(false);
    total = 0;
}

