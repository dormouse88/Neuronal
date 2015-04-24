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
#include "Device.hpp"
#include "Wire.hpp"
#include "AbstractFactory.hpp"
#include "Serializer.hpp"

class Model
{
public:
    Model();
    Model(const Model&) = delete;
    void SetFactory(std::shared_ptr<AbstractFactory> f) {theFactory = f; }
    void Logic();
    void SaveXML();
    void LoadXML();
    void SetPosition( Device & d, sf::Vector2i newPos );
    bool IsPositionFree(sf::Vector2i pos) const;
    bool IsWiringFree(Device & from, Device & to) const;
    
    void ImportDevice(std::shared_ptr<Device> device);
    void ExpelDevices();
    void ImportWire(std::shared_ptr<Wire> wire);
    void ExpelWires();
    
    std::shared_ptr<Device> GetDevice(sf::Vector2i pos);
    std::shared_ptr<Wire> GetWire(const Device& from, const Device& to);
    std::vector<std::shared_ptr<Wire> > GetWires(std::shared_ptr<Device> device, bool from, bool to); 
    
private:
    std::vector<std::shared_ptr<Device> > devices;
    std::vector<std::shared_ptr<Wire> > wires;
    std::shared_ptr<AbstractFactory> theFactory;
    std::shared_ptr<Serializer> theSerializer;
};

#endif	/* MODEL_HPP */

