/* 
 * File:   PlanPos.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 11:52
 */

#include "PlanPos.hpp"


PlanPos::PlanPos()
    :planGrid()
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
    if (planGrid.lock()) return true;
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
    pos = newPos;
    planOnly = false;
}
void PlanPos::SetPosDumb(VectorDumb newPos)
{
    pos = planGrid.lock()->MapDumbToSmart(newPos);
    planOnly = false;
}
void PlanPos::SetPosWorld(VectorWorld newPos)
{
    pos = planGrid.lock()->MapWorldtoSmart(newPos);
    planOnly = false;
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
    return lhs.pos == rhs.pos && lhs.planGrid.lock() == rhs.planGrid.lock() && lhs.planOnly == rhs.planOnly;
}



bool operator==(const PlanRect& lhs, const PlanRect& rhs)
{
    return lhs.br == rhs.br && lhs.tl == rhs.tl && lhs.valid == rhs.valid;
}
