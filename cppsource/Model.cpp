/* 
 * File:   Model.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:53
 */

#include "Model.hpp"

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
        NotifyListeners(true, rp);
    }
}
void Model::AddWire(Neuron & from, Neuron & to)
{
    bool wireExists = false;
    for (const auto & x: wires) {
        if (x->GetFrom() == from and x->GetTo() == to) wireExists = true;
    }
    if (wireExists == false) {
        std::unique_ptr<Wire> p{new Wire{from, to}};
        const Wire & cr = *p;
        wires.emplace_back(std::move(p));
        NotifyListeners(true, cr);
    }
}
void Model::AddWire(sf::Vector2i fromPos, sf::Vector2i toPos)
{
    Neuron * from = nullptr;
    Neuron * to   = nullptr;
    for (auto & x : neurons) {
        if (x->GetPosition() == fromPos) from = x.get();
        if (x->GetPosition() == toPos)   to   = x.get();
    }
    if (from != nullptr and to != nullptr) {
        AddWire(*from, *to);
    }
}

void Model::SetPosition( Neuron & n, sf::Vector2i newPos )
{
    bool posFree = true;
    for (auto const & x : neurons) {
        if (x->GetPosition() == newPos) {
            posFree = false;
        }
    }
    if (posFree) n.SetPosition( newPos );
}


Neuron * Model::GetNeuron(sf::Vector2i pos)
{
    for (auto & x: neurons) {
        if (pos == x->GetPosition()) {
            return x.get();
        }
    }
    return nullptr;
}
Wire * Model::GetWire(const Neuron& from, const Neuron& to)
{
    for (auto & x: wires) {
        if (from == x->GetFrom() and to == x->GetTo()) {
            return x.get();
        }
    }
    return nullptr;
}
Wire * Model::GetWire(sf::Vector2i fromPos, sf::Vector2i toPos)
{
    Neuron * from = nullptr;
    Neuron * to   = nullptr;
    for (auto & x : neurons) {
        if (x->GetPosition() == fromPos) from = x.get();
        if (x->GetPosition() == toPos)   to   = x.get();
    }
    if (from != nullptr and to != nullptr) {
        return GetWire(*from, *to);
    }
    return nullptr;
}


void Model::AddListener(ModelListener* listener)
{
    listeners.emplace_back(listener);
}

void Model::NotifyListeners(bool added, Neuron * rp)
{
    for (auto & x : listeners) {
        x->OnNotify(added, rp);
    }
}

void Model::NotifyListeners(bool added, const Wire & cr)
{
    for (auto & x : listeners) {
        x->OnNotify(added, cr);
    }
}
