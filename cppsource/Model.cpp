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
    for (auto & n : neurons) {
        n->StepPartOne();
    }
    for (auto & n : neurons) {
        n->StepPartTwo();
    }
    std::cout << "Vector sizes: " << neurons.size() << "  " << wires.size() << std::endl;
}

void Model::AddNeuron(sf::Vector2i pos)
{
    bool spotTaken = false;
    for (const auto & x : neurons) {
        if (x->GetPosition() == pos) spotTaken = true;
    }
    if (spotTaken == false) {
        std::unique_ptr<Neuron> p{new Neuron{pos}};
        Neuron * rp = p.get();
        neurons.emplace_back(std::move(p));
        NotifyListenersAdd(rp);
    }
}
void Model::RemoveDevice(sf::Vector2i pos)
{
    PinDevice * d = GetDevice(pos);
    if (d != nullptr) {
        //remove all wires joined to this neuron...
        auto remove_func = [&] (std::unique_ptr<Wire> & param) {return *d == param->GetFrom() or *d == param->GetTo(); };
        for (auto & w : wires) {
            if (remove_func(w)) {
                NotifyListenersRemove(*w);
                w->PreDelete();
            }
        }
        auto new_end1 = std::remove_if( std::begin(wires), std::end(wires), remove_func);
        wires.erase(new_end1, std::end(wires) );
        
        //then remove neuron itself...
        NotifyListenersRemove(d);
        auto new_end = std::remove_if(std::begin(neurons), std::end(neurons), [&] (std::unique_ptr<Neuron> & param) {return *d==*param;} );
        neurons.erase(new_end, std::end(neurons) );
    }
}

void Model::AddWire(PinDevice & from, PinDevice & to)
{
    bool wireExists = false;
    for (const auto & x: wires) {
        if (x->GetFrom() == from and x->GetTo() == to) wireExists = true;
    }
    if (wireExists == false) {
        std::unique_ptr<Wire> p{new Wire{from, to}};
        const Wire & cr = *p;
        wires.emplace_back(std::move(p));
        NotifyListenersAdd(cr);
    }
}
void Model::RemoveWire(PinDevice & from, PinDevice & to)
{
    Wire * w = GetWire(from, to);
    if (w != nullptr) {
        auto remove_func = [&] (std::unique_ptr<Wire> & param) {return *w==*param;};
        NotifyListenersRemove(*w);
        w->PreDelete();
        auto new_end = std::remove_if(std::begin(wires), std::end(wires), remove_func);
        wires.erase(new_end, std::end(wires) );
    }
}
void Model::AddWire(sf::Vector2i fromPos, sf::Vector2i toPos)
{
    PinDevice * from = nullptr;
    PinDevice * to   = nullptr;
    for (auto & x : neurons) {
        if (x->GetPosition() == fromPos) from = x.get();
        if (x->GetPosition() == toPos)   to   = x.get();
    }
    if (from != nullptr and to != nullptr) {
        AddWire(*from, *to);
    }
}

void Model::SetPosition( PinDevice & d, sf::Vector2i newPos )
{
    bool posFree = true;
    for (auto const & x : neurons) {
        if (x->GetPosition() == newPos) {
            posFree = false;
        }
    }
    if (posFree) d.SetPosition( newPos );
}


PinDevice * Model::GetDevice(sf::Vector2i pos)
{
    for (auto & x: neurons) {
        if (pos == x->GetPosition()) {
            return x.get();
        }
    }
    return nullptr;
}
Wire * Model::GetWire(const PinDevice& from, const PinDevice& to)
{
    for (auto & x: wires) {
        if (from == x->GetFrom() and to == x->GetTo()) {
            return x.get();
        }
    }
    return nullptr;
}
//Wire * Model::GetWire(sf::Vector2i fromPos, sf::Vector2i toPos)
//{
//    Neuron * from = nullptr;
//    Neuron * to   = nullptr;
//    for (auto & x : neurons) {
//        if (x->GetPosition() == fromPos) from = x.get();
//        if (x->GetPosition() == toPos)   to   = x.get();
//    }
//    if (from != nullptr and to != nullptr) {
//        return GetWire(*from, *to);
//    }
//    return nullptr;
//}


void Model::AddListener(ModelListener* listener)
{
    listeners.emplace_back(listener);
}

void Model::NotifyListenersAdd(Neuron * rp)
{
    for (auto & x : listeners) {
        x->OnNotifyAdd(rp);
    }
}

void Model::NotifyListenersAdd(const Wire & cr)
{
    for (auto & x : listeners) {
        x->OnNotifyAdd(cr);
    }
}

void Model::NotifyListenersRemove(PinDevice * rp)
{
    for (auto & x : listeners) {
        x->OnNotifyRemove(rp);
    }
}

void Model::NotifyListenersRemove(const Wire & cr)
{
    for (auto & x : listeners) {
        x->OnNotifyRemove(cr);
    }
}
