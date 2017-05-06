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
    BlobFactory(Shp<PlansDirectory> pgd) :planGroupData_(pgd) {}
    NeuronShp     AddNeuron(PlanPos pos);
    NeuronShp     AddNeuron(PlanShp plan, DevSerial, VectorSmart, int threshold, bool hasBulb);
    HandleShp     AddHandle(PlanPos pos);
    HandleShp     AddHandle(PlanShp plan, DevSerial, VectorSmart);
    
    PlanShp       MakePlan();
    Shp<BaseReferer> MakeBrain();
    
    WireShp       AddWire(Shp<WiringPair> wp, signed weight);

protected:
    std::shared_ptr<PlansDirectory> planGroupData_;
};



#endif	/* BLOBFACTORY_HPP */

