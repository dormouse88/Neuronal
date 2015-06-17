/* 
 * File:   NeuronCon.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 23:22
 */

#ifndef NEURONCON_HPP
#define	NEURONCON_HPP

#include <memory>
#include "DeviceCon.hpp"
#include "Neuron.hpp"

class NeuronCon : public DeviceCon
{
public:
    NeuronCon(std::shared_ptr<Neuron> neuron_p);
    virtual ~NeuronCon() {}
    
    virtual std::shared_ptr<const Device> GetDevice() const { return neuron_m.lock(); }
    virtual void Handle(int code);
    virtual bool IsDead() const { std::shared_ptr<const Neuron> sh {neuron_m.lock() } ; if (!sh) return true; return sh->IsDead();}
private:
    std::weak_ptr<Neuron> neuron_m;
};

#endif	/* NEURONCON_HPP */

