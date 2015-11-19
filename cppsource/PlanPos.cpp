/* 
 * File:   PlanPos.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 11:52
 */

#include "PlanPos.hpp"


PlanPos::PlanPos()
    :planGrid(nullptr)
{}

PlanPos::PlanPos(std::shared_ptr<PlanGrid> newGrid)
    :pos()
    ,planGrid(newGrid)
    ,planOnly(true)
{}

PlanPos::PlanPos(VectorSmart newPos, std::shared_ptr<PlanGrid> newGrid)
    :pos(newPos)
    ,planGrid(newGrid)
    ,planOnly(false)
{}

PlanPos::PlanPos(VectorWorld newPos, std::shared_ptr<PlanGrid> newGrid)
    :pos( newGrid->MapWorldtoSmart(newPos) )
    ,planGrid(newGrid)
    ,planOnly(false)
{}



void PlanPos::SetGrid(std::shared_ptr<PlanGrid> newGrid)
{
    planGrid = newGrid;
    planOnly = true;
}


bool PlanPos::IsValid() const
{
    if (planGrid) return true;
    else return false;
}
bool PlanPos::IsPlanOnly() const
{
    return planOnly;
}
bool PlanPos::IsLocated() const
{
    return not planOnly;
}
void PlanPos::SetPlanOnly()
{
    planOnly = true;
}


void PlanPos::SetPosSmart(VectorSmart newPos)
{
//    if (not planGrid) throw "SetPosSmart: no Grid";
    pos = newPos;
    planOnly = false;
}
void PlanPos::SetPosDumb(VectorDumb newPos)
{
//    if (not planGrid) throw "SetPosDumb: no Grid";
    pos = planGrid->MapDumbToSmart(newPos);
    planOnly = false;
}
void PlanPos::SetPosWorld(VectorWorld newPos)
{
//    if (not planGrid) throw "SetPosWorld: no Grid";
    pos = planGrid->MapWorldtoSmart(newPos);
    planOnly = false;
}

VectorSmart PlanPos::GetSmartPos() const
{
//    if (not planGrid) throw "SetSmartPos: no Grid";
    return pos;
}
VectorDumb PlanPos::GetDumbPos() const
{
//    if (not planGrid) throw "GetDumbPos: no Grid";
    return planGrid->MapSmartToDumb(pos);
}
VectorWorld PlanPos::GetWorldPos() const
{
//    if (not planGrid) throw "GetWorldPos: no Grid";
    return planGrid->MapSmartToWorld(pos);
}

VectorDumb PlanPos::GetDumbSizeOf() const
{
//    if (not planGrid) throw "GetDumbSizeOf: no Grid";
    return planGrid->DumbSizeOf(pos);
}
VectorWorld PlanPos::GetWorldSizeOf() const
{
//    if (not planGrid) throw "GetWorldSizeOf: no Grid";
    return planGrid->WorldSizeOf(pos);
}

