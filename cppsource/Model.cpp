/* 
 * File:   Model.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:53
 */

#include "Model.hpp"
#include <iostream>

Model::Model()
{}

void Model::Logic()
{
    for (auto & w : wires) {
        w->PushCharge();
    }
    for (auto & d : devices) {
        d->CalculateFiring();
    }
    for (auto & d : devices) {
        d->PushCharge();
    }
    std::cout << "Vector sizes: " << devices.size() << "  " << wires.size() << std::endl;
}


void Model::SetPosition( Device & d, sf::Vector2i newPos )
{
    bool posFree = true;
    for (auto const & x : devices) {
        if (x->GetPosition() == newPos) {
            posFree = false;
        }
    }
    if (posFree) d.SetPosition( newPos );
}

bool Model::IsPositionFree(sf::Vector2i pos) const
{
    for (const auto & x : devices) {
        if (x->GetPosition() == pos) return false;
    }
    return true;
}
bool Model::IsWiringFree(Device & from, Device & to) const
{
    for (const auto & x: wires) {
        if (x->GetFrom() == from and x->GetTo() == to) return false;
    }
    return true;
}

void Model::ImportDevice(std::shared_ptr<Device> device)
{
    devices.emplace_back(device);
}
void Model::ExpelDevices()
{
    auto remove_func = [] (std::shared_ptr<Device> eachDevice) {return eachDevice->IsDead();};
    auto new_end = std::remove_if(std::begin(devices), std::end(devices), remove_func );
    devices.erase(new_end, std::end(devices) );
}
void Model::ImportWire(std::shared_ptr<Wire> wire)
{
    wires.emplace_back(wire);
}
void Model::ExpelWires()
{
    auto remove_func = [] (std::shared_ptr<Wire> eachWire) {return eachWire->IsDead();};
    auto new_end = std::remove_if(std::begin(wires), std::end(wires), remove_func);
    wires.erase(new_end, std::end(wires) );
}

std::shared_ptr<Device> Model::GetDevice(sf::Vector2i pos)
{
    for (auto & x: devices) {
        if (pos == x->GetPosition()) {
            return x;
        }
    }
    return nullptr;
}
std::shared_ptr<Wire> Model::GetWire(const Device& from, const Device& to)
{
    for (auto & x: wires) {
        if (from == x->GetFrom() and to == x->GetTo()) {
            return x;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Wire> > Model::GetWires(std::shared_ptr<Device> device, bool from, bool to)
{
    std::vector<std::shared_ptr<Wire> > ret_vec;
    for (auto & x: wires) {
        if ((from and *device == x->GetFrom()) or (to and *device == x->GetTo())) {
            ret_vec.emplace_back(x);
        }
    }
    return ret_vec;
}

