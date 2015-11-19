/* 
 * File:   PlanGrid.cpp
 * Author: Dormouse
 * 
 * Created on 15 November 2015, 16:37
 */

#include "PlanGrid.hpp"
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

