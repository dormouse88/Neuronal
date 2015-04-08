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
    :PinDevice(pos_p, true), threshold(INITIAL_THRESHOLD), firing(false)
{}

void Neuron::StepPartOne()
{
    //get inputs and calculate firing bool
    int total = 0;
    for (const auto & w : inWires)
    {
        if (w->GetFiring()) total += w->GetWeight();
    }
    if (total >= threshold) firing = true;
    else firing = false;
}

void Neuron::StepPartTwo()
{
    //set outputs
    for (auto & w : outWires)
    {
        w->SetFiring(firing);
    }
}


