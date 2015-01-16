/* 
 * File:   Model.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:53
 */

#ifndef MODEL_HPP
#define	MODEL_HPP


#include <vector>
#include <memory>
#include "Neuron.hpp"
#include "Wire.hpp"
#include "ModelListener.hpp"

class Model {
public:
    Model();
    Model(const Model&) = delete;
    void Logic();
    void AddNeuron(sf::Vector2i pos);
    void AddWire(Neuron & from, Neuron & to);
    Neuron & GetNeuron(sf::Vector2i pos);
    
    void AddListener(ModelListener* listener);
private:
    void NotifyListeners(bool added, Neuron * rp);//, Component* component);
    std::vector<ModelListener*> listeners;
    std::vector<std::unique_ptr<Neuron> > neurons;
    std::vector<std::unique_ptr<Wire> > wires;

};

#endif	/* MODEL_HPP */

