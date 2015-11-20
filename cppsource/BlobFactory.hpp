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

namespace BlobFactory
{
    std::shared_ptr<Neuron>     AddNeuron(PlanPos pos);
    std::shared_ptr<Neuron>     AddNeuron(std::shared_ptr<ChipPlan> plan, int serial, VectorSmart pos, int threshold);
    std::shared_ptr<Jumper>     AddJumper(PlanPos pos);
    std::shared_ptr<Jumper>     AddJumper(std::shared_ptr<ChipPlan> plan, int serial, VectorSmart pos);
    std::shared_ptr<ChipHandle> AddHandle(PlanPos pos);
    std::shared_ptr<ChipHandle> AddHandle(std::shared_ptr<ChipPlan> plan, int serial, VectorSmart pos);
    
    std::shared_ptr<ChipPlan>   MakePlan();
    
    std::shared_ptr<Wire>       AddWire(PlanPos pos1, PlanPos pos2);
    std::shared_ptr<Wire>       AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, Wirable & to, signed weight);
    std::shared_ptr<Wire>       AddWire(std::shared_ptr<ChipPlan> plan, Wirable & from, int fromSlot, Wirable & to, int toSlot, signed weight);

    void RemoveDevice(PlanPos pos);
    void RemoveWire(PlanPos pos1, PlanPos pos2);

};

#endif	/* BLOBFACTORY_HPP */

