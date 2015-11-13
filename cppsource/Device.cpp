/* 
 * File:   Device.cpp
 * Author: Dormouse
 *
 * Created on 24 April 2015, 15:09
 */

#include "Device.hpp"
#include "ChipPlan.hpp"

Device::Device(int serial_p, VectorSmart pos_p, std::shared_ptr<ChipPlan> cont)
    :Gobject(cont)
    , serial(serial_p)
    , ppos(pos_p, cont->GetGrid())
{}

VectorWorld Device::CalculateOffset(VectorWorld objectSize) const
{
    return VectorWorld { GetWorldPos() + (GetWorldSizeOfCell() - objectSize)/2.f };
}
