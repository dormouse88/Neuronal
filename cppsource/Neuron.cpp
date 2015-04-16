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
    :Device(pos_p), threshold(INITIAL_THRESHOLD), firing(false), total(0)
{}

void Neuron::ReceiveCharge(int weight)
{
    total += weight;
}

void Neuron::PushCharge()
{
    for (auto & w: outWires)
    {
        w.lock()->ReceiveCharge(firing);
    }
}

void Neuron::CalculateFiring()
{
    if (total >= threshold) firing = true;
    else firing = false;
    total = 0;
}

