/* 
 * File:   Controller.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 12:42
 */

#ifndef CONTROLLER_HPP
#define	CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"
#include "AbstractFactory.hpp"
#include "DeviceCon.hpp"
#include "WireCon.hpp"

class Controller
{
public:
    Controller(Model & model_p, View & view_p);
    Controller(const Controller&) = delete;
    bool HandleInput();
    void SetFactory(std::shared_ptr<AbstractFactory> f) {theFactory = f;}

    std::shared_ptr<DeviceCon> GetDevice(std::shared_ptr<Device> d);
    std::shared_ptr<WireCon> GetWire(std::shared_ptr<Wire> w);
    
    void ImportDevice(std::shared_ptr<DeviceCon> device);
    void ExpelDevices();
    void ImportWire(std::shared_ptr<WireCon> wire);
    void ExpelWires();
private:
    Model & theModel;
    View & theView;
    std::shared_ptr<AbstractFactory> theFactory;

    std::vector<std::shared_ptr<DeviceCon> > deviceCons;
    std::vector<std::shared_ptr<WireCon> > wireCons;    
    
    sf::Vector2f mouseCursorWorldPos;
    bool mouseCursorSet;
};

#endif	/* CONTROLLER_HPP */

