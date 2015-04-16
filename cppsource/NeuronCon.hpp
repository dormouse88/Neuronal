/* 
 * File:   NeuronCon.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 23:22
 */

#ifndef NEURONCON_HPP
#define	NEURONCON_HPP

#include "DeviceCon.hpp"
#include "Neuron.hpp"

class NeuronCon : public DeviceCon
{
public:
    NeuronCon(Neuron & neuron_p);
    virtual ~NeuronCon() {}
    
    virtual const Device& GetDevice() const { return neuron_m; }
    virtual void Handle(int code);
    virtual bool IsDead() const {return neuron_m.IsDead();}
private:
    Neuron & neuron_m;
};

#endif	/* NEURONCON_HPP */

