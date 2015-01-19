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

void Model::SetThreshold( sf::Vector2i pos, unsigned val )
{
    for (auto & x: neurons) {
        if (pos == x->GetPosition() ) {
            x->SetThreshold(val);
        }
    }
}

void Model::ModifyThreshold( sf::Vector2i pos, int val )
{
    for (auto & x: neurons) {
        if (pos == x->GetPosition() ) {
            x->SetThreshold( x->GetThreshold() + val );
        }
    }
}

void Model::SetPosition( sf::Vector2i pos, sf::Vector2i newPos )
{
    for (auto & x: neurons) {
        if (pos == x->GetPosition() ) {
            x->SetPosition( newPos );
        }
    }
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
