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
    std::unique_ptr<Neuron> p{new Neuron{pos}};
    Neuron * rp = p.get();
    neurons.emplace_back(std::move(p));
    NotifyListeners(true, rp);
}

void Model::AddWire(Neuron & from, Neuron & to)
{
    std::unique_ptr<Wire> p{new Wire{from, to}};
    wires.emplace_back(std::move(p));
}

Neuron & Model::GetNeuron(sf::Vector2i pos)
{
    for (auto & x : neurons) {
        if (pos == x->GetPosition() ) {
           return *x;
        }
    }
}


void Model::AddListener(ModelListener* listener)
{
    listeners.emplace_back(listener);
}

void Model::NotifyListeners(bool added, Neuron * rp)//, Component* component)
{
    for (auto & x : listeners) {
        x->OnNotify(added, rp);//, component);
    }
}

