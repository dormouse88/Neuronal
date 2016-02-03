/* 
 * File:   PlanPos.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 11:52
 */

#include "PlanPos.hpp"
#include "ChipPlan.hpp"  //fwd dec
#include "ChipHandle.hpp"


PlanPos::PlanPos(VectorSmart newPos, std::shared_ptr<PlanGrid> newGrid)
    :pos(newPos)
    ,planGrid(newGrid)
{}

PlanPos::PlanPos(VectorWorld newPos, std::shared_ptr<PlanGrid> newGrid)
    :pos( newGrid->MapWorldtoSmart(newPos) )
    ,planGrid(newGrid)
{}



void PlanPos::SetPosSmart(VectorSmart newPos)
{
    pos = newPos;
}
void PlanPos::SetPosDumb(VectorDumb newPos)
{
    pos = planGrid.lock()->MapDumbToSmart(newPos);
}
void PlanPos::SetPosWorld(VectorWorld newPos)
{
    pos = planGrid.lock()->MapWorldtoSmart(newPos);
}

VectorSmart PlanPos::GetSmartPos() const
{
    return pos;
}
VectorDumb PlanPos::GetDumbPos() const
{
    return planGrid.lock()->MapSmartToDumb(pos);
}
VectorWorld PlanPos::GetWorldPos() const
{
    return planGrid.lock()->MapSmartToWorld(pos);
}

VectorDumb PlanPos::GetDumbSizeOf() const
{
    return planGrid.lock()->DumbSizeOf(pos);
}
VectorWorld PlanPos::GetWorldSizeOf() const
{
    return planGrid.lock()->WorldSizeOf(pos);
}



bool operator==(const PlanPos& lhs, const PlanPos& rhs)
{
    return lhs.pos == rhs.pos && lhs.planGrid.lock() == rhs.planGrid.lock();
}





DeviceShp PlanPos::GetDevice()
{
    return GetPlan()->GetDevice(GetSmartPos());
}
HandleShp PlanPos::GetDeviceAsHandle()
{
    auto d = GetDevice();
    return std::dynamic_pointer_cast<ChipHandle> (d);
}
bool PlanPos::IsPositionFree()
{
    return GetPlan()->IsPositionFree(GetSmartPos());
}










bool operator==(const PlanRect& lhs, const PlanRect& rhs)
{
    return lhs.br == rhs.br && lhs.tl == rhs.tl; // && lhs.valid == rhs.valid;
}
