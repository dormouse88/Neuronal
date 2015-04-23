/* 
 * File:   Factory.hpp
 * Author: Dormouse
 *
 * Created on 08 April 2015, 23:31
 */

#ifndef FACTORY_HPP
#define	FACTORY_HPP

#include "Wire.hpp"
#include "Neuron.hpp"
#include "Socket.hpp"
#include "Model.hpp"
#include "View.hpp"
#include "Controller.hpp"
#include "AbstractFactory.hpp"

class Factory : public AbstractFactory
{
public:
    Factory(Model & m, View & v, Controller & c);
    virtual ~Factory() {}

    virtual void AddNeuron(sf::Vector2i pos);
    virtual void AddSocket(sf::Vector2i pos);
    virtual void RemoveDevice(std::shared_ptr<Device> device);
    virtual void AddWire(Device & from, Device & to);
    virtual void RemoveWire(std::shared_ptr<Wire> wire);
private:
    Model & model;
    View & view;
    Controller & controller;
    ViewResources vRes;
};

#endif	/* FACTORY_HPP */

