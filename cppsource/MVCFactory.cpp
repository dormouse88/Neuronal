/* 
 * File:   MVCFactory.cpp
 * Author: Dormouse
 * 
 * Created on 08 April 2015, 23:31
 */

#include "MVCFactory.hpp"
#include "NeuronView.hpp"
#include "NeuronCon.hpp"
#include "SocketView.hpp"
#include "WireView.hpp"
#include "WireCon.hpp"

MVCFactory::MVCFactory(Model & m, View & v, Controller & c)
    :FactoryBase(), model(m), view(v), controller(c)
{}

void MVCFactory::ExpelDead()
{
    controller.ExpelWires();
    controller.ExpelDevices();
    view.ExpelWires();
    view.ExpelDevices();
    model.ExpelWires();
    model.ExpelDevices();
}

void MVCFactory::ClearEverything()
{
    model.ClearEverything();
}

void MVCFactory::AddNeuron(int serial, sf::Vector2i pos, int threshold)
{
    if (serial == 0) serial = model.GetFreeSerial();
    if (model.IsPositionFree(pos) and model.IsSerialFree(serial))
    {
        auto mp = std::make_shared<Neuron> (serial, pos, threshold);
        model.ImportDevice(mp);
        auto vp = std::make_shared<NeuronView> (mp, vRes);
        view.ImportDevice(vp);
        auto cp = std::make_shared<NeuronCon> (mp);
        controller.ImportDevice(cp);
    }
}

void MVCFactory::AddSocket(int serial, sf::Vector2i pos)
{
    if (serial == 0) serial = model.GetFreeSerial();
    if (model.IsPositionFree(pos) and model.IsSerialFree(serial))
    {
        auto mp = std::make_shared<Socket> (serial, pos);
        model.ImportDevice(mp);
        auto vp = std::make_shared<SocketView> (mp, vRes);
        view.ImportDevice(vp);
//        auto cp = std::make_shared<SocketCon> (*mp);
//        controller.ImportDevice(cp);
    }
}

void MVCFactory::RemoveDevice(std::shared_ptr<Device> device)
{
    if (device != nullptr) {
        for (auto w : model.GetWires(device, true, true))
        {
            w->Zingaya();
        }
        device->Zingaya();
        ExpelDead();
    }
}

void MVCFactory::AddHandle(int serial, sf::Vector2i pos)
{
    if (serial == 0) serial = model.GetFreeSerial();
    if (model.IsPositionFree(pos) and model.IsSerialFree(serial))
    {
        //auto mp = std::make_shared<ChipHandle> (serial, pos);
        //model.ImportDevice(mp);
//        auto vp = std::make_shared<NeuronView> (mp, vRes);
//        view.ImportDevice(vp);
//        auto cp = std::make_shared<NeuronCon> (mp);
//        controller.ImportDevice(cp);
    }
}

void MVCFactory::AddPlan()
{
    //auto mp = std::make_shared<ChipPlan> ();
}

void MVCFactory::AddWire(Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight)
{
    if (model.IsWiringFree(from, fromSlot, to, toSlot) and from.CanRegisterOut(fromSlot) and to.CanRegisterIn(toSlot)) //change IsWiringFree() to include fromSlot and toSlot parameters
    {
        auto mp = std::make_shared<Wire> (from, to, weight);
        from.RegisterOut(mp);
        to.RegisterIn(mp);
        model.ImportWire(mp);
        auto vp = std::make_shared<WireView> (mp, vRes);
        view.ImportWire(vp);
        auto cp = std::make_shared<WireCon> (mp);
        controller.ImportWire(cp);
    }
}
//void MVCFactory::AddWire(int from, int fromSlot, int to, int toSlot, signed weight)
//{
//    auto f = model.GetDevice(from);
//    auto t = model.GetDevice(to);
//    if (f && t) AddWire(*f, fromSlot, *t, toSlot, weight);
//}
void MVCFactory::RemoveWire(std::shared_ptr<Wire> wire)
{
    if (wire != nullptr) {
        wire->Zingaya();
        ExpelDead();
    }
}

