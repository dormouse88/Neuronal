/* 
 * File:   PlanPos.hpp
 * Author: Dormouse
 *
 * Created on 11 November 2015, 11:52
 */

#ifndef PLANPOS_HPP
#define	PLANPOS_HPP

#include <memory>
#include "PlanGrid.hpp"

class PlanPos
{
public:
    PlanPos();
    PlanPos(std::shared_ptr<PlanGrid>);
    PlanPos(VectorSmart, std::shared_ptr<PlanGrid>);
    PlanPos(VectorWorld, std::shared_ptr<PlanGrid>);

    std::shared_ptr<PlanGrid> GetGrid() { return planGrid.lock(); }
    std::shared_ptr<ChipPlan> GetPlan() { auto l = planGrid.lock(); return l ? l->GetPlan() : nullptr; }
    void SetGrid(std::shared_ptr<PlanGrid>);
    
    bool IsValid() const;
    bool IsPlanOnly() const;
    bool IsLocated() const;
    void Dislocate();

    void SetPosSmart(VectorSmart);
    void SetPosDumb(VectorDumb);
    void SetPosWorld(VectorWorld);

    VectorSmart GetSmartPos() const;
    VectorDumb GetDumbPos() const;
    VectorWorld GetWorldPos() const;
    
    VectorDumb GetDumbSizeOf() const;
    VectorWorld GetWorldSizeOf() const;
private:
    VectorSmart pos;
    std::weak_ptr<PlanGrid> planGrid;
    bool planOnly;

    friend bool operator== (const PlanPos& lhs, const PlanPos& rhs);
};

bool operator==(const PlanPos& lhs, const PlanPos& rhs);


struct PlanRect
{
    PlanRect()
        :valid(false)
    {}
    PlanRect(PlanPos tl_, PlanPos br_)
        :tl(tl_), br(br_), valid(true)
    {}
    PlanRect AddPadding(int thickness)
    {
        tl.SetPosSmart( tl.GetSmartPos() - VectorSmart{ thickness, thickness } );
        br.SetPosSmart( br.GetSmartPos() + VectorSmart{ thickness, thickness } );
        return *this;
    }
    RectDumb GetRectDumb() const
    {
        return RectDumb { tl.GetDumbPos(), br.GetDumbPos() - tl.GetDumbPos() + br.GetDumbSizeOf() };
    }
    RectWorld GetRectWorld() const
    {
        return RectWorld { tl.GetWorldPos(), br.GetWorldPos() - tl.GetWorldPos() + br.GetWorldSizeOf() };
    }
//    RectWorld GetRectWorldWithCellMiddles() const
//    {
//        return RectWorld { tl.GetWorldPos() + VectorWorld{ tl.GetWorldSizeOf().x * 0.5f, 0.f},
//                br.GetWorldPos() - tl.GetWorldPos() + br.GetWorldSizeOf() - VectorWorld{br.GetWorldSizeOf().x, 0.f} };
//    }
    void SetGrid(std::shared_ptr<PlanGrid> newGrid) 
    {
        tl.SetGrid(newGrid);
        br.SetGrid(newGrid);
    }
    PlanPos tl;
    PlanPos br;
    bool valid;
};

bool operator==(const PlanRect& lhs, const PlanRect& rhs);

#endif	/* PLANPOS_HPP */

