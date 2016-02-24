/* 
 * File:   Device.cpp
 * Author: Dormouse
 *
 * Created on 24 April 2015, 15:09
 */

#include "Device.hpp"
#include "ChipPlan.hpp"

Device::Device(DevSerial ds, VectorSmart pos, PlanShp cont)
    :PlanOwned(cont)
    , ds_(ds)
    , ppos_(pos, cont->GetGrid())
{}

VectorWorld Device::CalculateOffsetForCentering(VectorWorld objectSize) const
{
    return VectorWorld { GetWorldPos() + (GetWorldSizeOfCell() - objectSize)/2.f };
}
