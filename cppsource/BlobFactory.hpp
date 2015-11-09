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
#include "Jumper.hpp"
#include "ChipHandle.hpp"
#include "ChipPlan.hpp"

class BlobFactory
{
public:
    BlobFactory();
    ~BlobFactory() {}

    void AddNeuron(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos, int threshold);
    void AddJumper(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos);
    std::shared_ptr<ChipHandle> AddHandle(std::shared_ptr<ChipPlan> plan, int serial, sf::Vector2i pos);
    //std::shared_ptr<ChipPlan> MakePlan(std::shared_ptr<ChipHandle> handle);
    void AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, Wirable & to, signed weight);
    void AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight);
    //void AddWire(int from, int fromSlot, int to, int toSlot, signed weight);

private:
};

#endif	/* BLOBFACTORY_HPP */

