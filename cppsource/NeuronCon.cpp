/* 
 * File:   NeuronCon.cpp
 * Author: Dormouse
 * 
 * Created on 14 April 2015, 23:22
 */

#include "NeuronCon.hpp"

NeuronCon::NeuronCon(Neuron & neuron_p)
    :DeviceCon(),
    neuron_m(neuron_p)
{}

void NeuronCon::Handle(int code)
{
    if (code == 1) { neuron_m.ModifyThreshold(1); }
    if (code == 2) { neuron_m.ModifyThreshold(-1); }
}