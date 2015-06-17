/* 
 * File:   FactoryBase.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 16:24
 */

#ifndef FACTORYBASE_HPP
#define	FACTORYBASE_HPP

#include "Wire.hpp"
#include "Neuron.hpp"

class FactoryBase
{
public:
    FactoryBase() {}
    virtual ~FactoryBase() {}

    virtual void ExpelDead() = 0;
    virtual void ClearEverything() = 0;
    virtual void AddNeuron(int serial, sf::Vector2i pos, int threshold) = 0;
    virtual void AddSocket(int serial, sf::Vector2i pos) = 0;
    virtual void RemoveDevice(std::shared_ptr<Device> device) = 0;
    virtual void AddWire(Device & from, Device & to, signed weight) = 0;
    virtual void AddWire(int from, int to, signed weight) = 0;    
    virtual void RemoveWire(std::shared_ptr<Wire> wire) = 0;
private:
};

#endif	/* FACTORYBASE_HPP */

