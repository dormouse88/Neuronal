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

class Model
{
public:
    Model();
    Model(const Model&) = delete;
    void Logic();
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

};

#endif	/* MODEL_HPP */

