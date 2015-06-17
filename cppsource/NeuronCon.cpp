/* 
 * File:   NeuronCon.cpp
 * Author: Dormouse
 * 
 * Created on 14 April 2015, 23:22
 */

#include "NeuronCon.hpp"

NeuronCon::NeuronCon(std::shared_ptr<Neuron> neuron_p)
    :DeviceCon(),
    neuron_m(neuron_p)
{}

void NeuronCon::Handle(int code)
{
    std::shared_ptr<Neuron> m {neuron_m.lock()};
    if (m) {
        if (code == 1) { m->ModifyThreshold(1); }
        if (code == 2) { m->ModifyThreshold(-1); }
    }
}