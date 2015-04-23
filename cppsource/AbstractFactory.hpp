/* 
 * File:   AbstractFactory.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 16:24
 */

#ifndef ABSTRACTFACTORY_HPP
#define	ABSTRACTFACTORY_HPP

#include "Wire.hpp"
#include "Neuron.hpp"

class AbstractFactory
{
public:
    AbstractFactory() {}
    virtual ~AbstractFactory() {}

    virtual void AddNeuron(sf::Vector2i pos) = 0;
    virtual void AddSocket(sf::Vector2i pos) = 0;
    virtual void RemoveDevice(std::shared_ptr<Device> device) = 0;
    virtual void AddWire(Device & from, Device & to) = 0;
    virtual void RemoveWire(std::shared_ptr<Wire> wire) = 0;
private:
};

#endif	/* ABSTRACTFACTORY_HPP */

