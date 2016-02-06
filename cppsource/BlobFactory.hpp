/* 
 * File:   BlobFactory.hpp
 * Author: Dormouse
 *
 * Created on 31 July 2015
 */

#ifndef BLOBFACTORY_HPP
#define	BLOBFACTORY_HPP

#include "BasicTypes.hpp"
#include "WiringPair.hpp"
#include "Wire.hpp"
#include "Neuron.hpp"
#include "ChipHandle.hpp"
#include "ChipPlan.hpp"
#include "BaseReferer.hpp"

class BlobFactory
{
public:
    BlobFactory(Shp<UserData> ud) :userData_(ud) {}
    NeuronShp     AddNeuron(PlanPos pos);
    NeuronShp     AddNeuron(PlanShp plan, int serial, VectorSmart pos, int threshold, bool hasBulb);
    HandleShp     AddHandle(PlanPos pos);
    HandleShp     AddHandle(PlanShp plan, int serial, VectorSmart pos);
    
    PlanShp       MakePlan();
    Shp<BaseReferer> MakeBrain();
    
    WireShp       AddWire(Shp<WiringPair> wp, signed weight);

//    void RemoveDevice(PlanPos pos);
//    void RemoveWire(Shp<WiringPair> wp);

protected:
    std::shared_ptr<UserData> userData_;
};



#endif	/* BLOBFACTORY_HPP */

