/* 
 * File:   PlanPos.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 11:52
 */

#include "PlanPos.hpp"
#include "ViewResources.hpp" //for GRID_SIZE


VectorDumb PlanGrid::MapSmartToDumb(VectorSmart point) const
{
    return VectorDumb {
        xPansions.MapSmartToDumb(point.x),
        yPansions.MapSmartToDumb(point.y)
    };
}
VectorSmart PlanGrid::MapDumbToSmart(VectorDumb dumb) const
{
    return VectorSmart {
        xPansions.MapDumbToSmart( dumb.x ),
        yPansions.MapDumbToSmart( dumb.y )
    };
}

VectorWorld PlanGrid::MapDumbToWorld(VectorDumb dumb) const
{
    return VectorWorld {
        dumb.x * GRID_SIZE.x,
        dumb.y * GRID_SIZE.y
    } + offset;
}
VectorDumb PlanGrid::MapWorldToDumb(VectorWorld world) const
{
    VectorWorld moved { world - offset };
    return VectorDumb {
        static_cast<int>(floorf(moved.x / GRID_SIZE.x)),
        static_cast<int>(floorf(moved.y / GRID_SIZE.y))
    };
}

VectorWorld PlanGrid::MapSmartToWorld(VectorSmart smart) const
{
    return MapDumbToWorld( MapSmartToDumb(smart) );
}
VectorSmart PlanGrid::MapWorldtoSmart(VectorWorld world) const
{
    return MapDumbToSmart( MapWorldToDumb(world) );
}



VectorDumb PlanGrid::DumbSizeOf(VectorSmart point) const
{
    return VectorDumb {
        xPansions.GetSize(point.x),
        yPansions.GetSize(point.y)
    };
}
VectorWorld PlanGrid::WorldSizeOf(VectorSmart point) const
{
    VectorDumb dumb { DumbSizeOf(point) };
    return VectorWorld {
        dumb.x * GRID_SIZE.x,
        dumb.y * GRID_SIZE.y
    };
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






PlanPos::PlanPos()
    :planGrid(nullptr)
{}

PlanPos::PlanPos(VectorSmart newPos, std::shared_ptr<PlanGrid> newGrid)
    :pos(newPos)
    ,planGrid(newGrid)
{}

PlanPos::PlanPos(VectorWorld newPos, std::shared_ptr<PlanGrid> newGrid)
    :pos( newGrid->MapWorldtoSmart(newPos) )
    ,planGrid(newGrid)
{}

void PlanPos::SetGrid(std::shared_ptr<PlanGrid> newGrid)
{
    planGrid = newGrid;
}

bool PlanPos::IsValid() const
{
    if (planGrid) return true;
    else return false;
}

void PlanPos::SetPosSmart(VectorSmart newPos)
{
    if (not planGrid) throw "no Grid";
    pos = newPos;
}
void PlanPos::SetPosDumb(VectorDumb newPos)
{
    if (not planGrid) throw "no Grid";
    pos = planGrid->MapDumbToSmart(newPos);
}
void PlanPos::SetPosWorld(VectorWorld newPos)
{
    if (not planGrid) throw "no Grid";
    pos = planGrid->MapWorldtoSmart(newPos);
}

VectorSmart PlanPos::GetSmartPos() const
{
    if (not planGrid) throw "no Grid";
    return pos;
}
VectorDumb PlanPos::GetDumbPos() const
{
    if (not planGrid) throw "no Grid";
    return planGrid->MapSmartToDumb(pos);
}
VectorWorld PlanPos::GetWorldPos() const
{
    if (not planGrid) throw "no Grid";
    return planGrid->MapSmartToWorld(pos);
}

VectorDumb PlanPos::GetDumbSizeOf() const
{
    if (not planGrid) throw "no Grid";
    return planGrid->DumbSizeOf(pos);
}
VectorWorld PlanPos::GetWorldSizeOf() const
{
    if (not planGrid) throw "no Grid";
    return planGrid->WorldSizeOf(pos);
}

