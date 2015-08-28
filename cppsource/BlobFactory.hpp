/* 
 * File:   BlobFactory.hpp
 * Author: Dormouse
 *
 * Created on 31 July 2015
 */

#ifndef BLOBFACTORY_HPP
#define	BLOBFACTORY_HPP

#include "Wire.hpp"
#include "Neuron.hpp"
#include "Socket.hpp"
#include "ChipHandle.hpp"
#include "ChipPlan.hpp"

class BlobFactory
{
public:
    BlobFactory();
    virtual ~BlobFactory() {}

    virtual void AddNeuron(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos, int threshold);
    virtual void AddSocket(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos);
    virtual void AddHandle(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos);
    virtual void AddPlan();
    virtual void AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight);
    //virtual void AddWire(int from, int fromSlot, int to, int toSlot, signed weight);

    virtual void RemoveDevice(std::shared_ptr<ChipPlan> plan, std::shared_ptr<Device> device);
    virtual void RemoveWire(std::shared_ptr<ChipPlan> plan, std::shared_ptr<Wire> wire);
private:
};

#endif	/* BLOBFACTORY_HPP */

