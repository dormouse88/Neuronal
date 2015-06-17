/* 
 * File:   MVCFactory.hpp
 * Author: Dormouse
 *
 * Created on 08 April 2015, 23:31
 */

#ifndef MVCFACTORY_HPP
#define	MVCFACTORY_HPP

#include "Wire.hpp"
#include "Neuron.hpp"
#include "Socket.hpp"
#include "Model.hpp"
#include "View.hpp"
#include "Controller.hpp"
#include "FactoryBase.hpp"

class MVCFactory : public FactoryBase
{
public:
    MVCFactory(Model & m, View & v, Controller & c);
    virtual ~MVCFactory() {}

    virtual void ExpelDead();
    virtual void ClearEverything();
    virtual void AddNeuron(int serial, sf::Vector2i pos, int threshold);
    virtual void AddSocket(int serial, sf::Vector2i pos);
    virtual void RemoveDevice(std::shared_ptr<Device> device);
    virtual void AddWire(Device & from, Device & to, signed weight);
    virtual void AddWire(int from, int to, signed weight);
    virtual void RemoveWire(std::shared_ptr<Wire> wire);
private:
    Model & model;
    View & view;
    Controller & controller;
    ViewResources vRes;
};

#endif	/* MVCFACTORY_HPP */

