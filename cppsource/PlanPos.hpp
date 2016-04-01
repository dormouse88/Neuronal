/* 
 * File:   PlanPos.hpp
 * Author: Dormouse
 *
 * Created on 11 November 2015, 11:52
 */

#ifndef PLANPOS_HPP
#define	PLANPOS_HPP

#include <memory>
class ChipPlan; //fwd dec
#include "PlanGrid.hpp"


enum class PlanAddressMode { ABSENT, PLANONLY, CELL, PORT };
struct PlanAddress
{
    PlanAddress() :mode(PlanAddressMode::ABSENT) {}
    PlanAddressMode mode;
    std::weak_ptr<ChipPlan> plan;
    VectorSmart pos;
    PortLocation port;
};


class PlanPos
{
public:
    PlanPos(VectorSmart, std::shared_ptr<PlanGrid>);
    PlanPos(VectorWorld, std::shared_ptr<PlanGrid>);

    std::shared_ptr<PlanGrid> GetGrid()             { return planGrid.lock(); }
    PlanShp GetPlan()             { auto l = planGrid.lock(); return l ? l->GetPlan() : nullptr; }
    void SetGrid(std::shared_ptr<PlanGrid> newGrid) { planGrid = newGrid;}
    
    void SetPosSmart(VectorSmart);
    void SetPosDumb(VectorDumb);
    void SetPosWorld(VectorWorld);

    VectorSmart GetSmartPos() const;
    VectorDumb GetDumbPos() const;
    VectorWorld GetWorldPos() const;
    
    VectorDumb GetDumbSizeOf() const;
    VectorWorld GetWorldSizeOf() const;
    
    DeviceShp GetDevice();
    HandleShp GetDeviceAsHandle();
    bool IsPositionFree();
private:
    VectorSmart pos;
    std::weak_ptr<PlanGrid> planGrid;

    friend bool operator== (const PlanPos& lhs, const PlanPos& rhs);
};

bool operator==(const PlanPos& lhs, const PlanPos& rhs);


















struct PlanRect
{
    PlanRect(PlanPos tl_, PlanPos br_)
        :tl(tl_), br(br_)
    {}
//    PlanRect AddPadding(int thickness)
//    {
//        tl.SetPosSmart( tl.GetSmartPos() - VectorSmart{ thickness, thickness } );
//        br.SetPosSmart( br.GetSmartPos() + VectorSmart{ thickness, thickness } );
//        return *this;
//    }
    RectDumb GetRectDumb() const
    {
        return RectDumb { tl.GetDumbPos(), br.GetDumbPos() - tl.GetDumbPos() + br.GetDumbSizeOf() };
    }
    RectWorld GetRectWorld() const
    {
        return RectWorld { tl.GetWorldPos(), br.GetWorldPos() - tl.GetWorldPos() + br.GetWorldSizeOf() };
    }
    void SetGrid(std::shared_ptr<PlanGrid> newGrid) 
    {
        tl.SetGrid(newGrid);
        br.SetGrid(newGrid);
    }
    PlanPos tl;
    PlanPos br;
};

bool operator==(const PlanRect& lhs, const PlanRect& rhs);

#endif	/* PLANPOS_HPP */

