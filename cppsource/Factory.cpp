/* 
 * File:   Factory.cpp
 * Author: Dormouse
 * 
 * Created on 08 April 2015, 23:31
 */

#include "Factory.hpp"
#include "NeuronView.hpp"
#include "NeuronCon.hpp"
#include "WireView.hpp"
#include "WireCon.hpp"

Factory::Factory(Model & m, View & v, Controller & c)
    :AbstractFactory(), model(m), view(v), controller(c)
{}

void Factory::AddNeuron(sf::Vector2i pos)
{
    if (model.IsPositionFree(pos))
    {
        auto mp = std::make_shared<Neuron> (pos);
        model.ImportDevice(mp);
        auto vp = std::make_shared<NeuronView> (*mp, vRes);
        view.ImportDevice(vp);
        auto cp = std::make_shared<NeuronCon> (*mp);
        controller.ImportDevice(cp);
    }
}

void Factory::RemoveDevice(std::shared_ptr<Device> device)
{
    if (device != nullptr) {
        for (auto w : model.GetWires(device, true, true))
        {
            w->Zingaya();
        }
        device->Zingaya();
        model.ExpelWires();
        view.ExpelWires();
        controller.ExpelWires();
        model.ExpelDevices();
        view.ExpelDevices();
        controller.ExpelDevices();
    }
}

void Factory::AddWire(Device & from, Device & to)
{
    if (model.IsWiringFree(from, to))
    {
        auto mp = std::make_shared<Wire> (from, to);
        from.RegisterOut(mp);
        to.RegisterIn(mp);
        model.ImportWire(mp);
        auto vp = std::make_shared<WireView> (*mp, vRes);
        view.ImportWire(vp);
        auto cp = std::make_shared<WireCon> (*mp);
        controller.ImportWire(cp);
    }
}
void Factory::RemoveWire(std::shared_ptr<Wire> wire)
{
    if (wire != nullptr) {
        wire->Zingaya();
        model.ExpelWires();
        view.ExpelWires();
        controller.ExpelWires();
    }
}

