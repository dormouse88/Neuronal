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
    void RemoveDevice(sf::Vector2i pos);
    void AddWire(PinDevice & from, PinDevice & to);
    void AddWire(sf::Vector2i from, sf::Vector2i to); //Only needed by MakeSomeStuff(). Phase out
    void RemoveWire( PinDevice & from, PinDevice & to);
    void SetPosition( PinDevice & d, sf::Vector2i newPos );

    PinDevice * GetDevice(sf::Vector2i pos);
    Wire * GetWire(const PinDevice& from, const PinDevice& to);
    //Wire * GetWire(sf::Vector2i fromPos, sf::Vector2i toPos);
    
    void AddListener(ModelListener* listener);
private:
    void NotifyListenersAdd(Neuron * rp);
    void NotifyListenersAdd(const Wire & cr);
    void NotifyListenersRemove(PinDevice * rp);
    void NotifyListenersRemove(const Wire & cr);
    std::vector<ModelListener*> listeners;
    std::vector<std::unique_ptr<Neuron> > neurons;
    std::vector<std::unique_ptr<Wire> > wires;

};

#endif	/* MODEL_HPP */

