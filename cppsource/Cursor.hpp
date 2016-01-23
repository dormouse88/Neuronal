/* 
 * File:   Cursor.hpp
 * Author: Dormouse
 *
 * Created on 18 January 2015, 20:37
 */

#ifndef CURSOR_HPP
#define	CURSOR_HPP

#include <memory>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "ChipPlan.hpp"  //class PlanGrid;
#include "PlanPos.hpp"

//move this somewhere better...
struct PortData 
{
    bool portSide_;
    int portNumber_;
};


enum class CursorState { ABSENT, PLAN, LOCATED, PORT };

class Cursor
{
public:
    Cursor(std::shared_ptr<PlanGrid> g, sf::Color color);
    void Draw(sf::RenderTarget & rt);

    CursorState GetState() const                        { return cursorState_; }
    PlanShp GetPlan()                                   { assert(cursorState_ != CursorState::ABSENT); return plan_; }
    PlanPos GetPlanPos()                                { assert(cursorState_ == CursorState::LOCATED); return PlanPos(pos_, plan_->GetGrid()); }
    
    void SetToAbsent()
        { cursorState_ = CursorState::ABSENT; }
    void SetToPlan()
        { SetToPlan(plan_); }
    void SetToPlan(PlanShp plan)
        { assert(plan); cursorState_ = CursorState::PLAN; plan_ = plan;}
    void SetToLocated(VectorSmart sp)
        { assert(plan_); cursorState_ = CursorState::LOCATED; pos_ = sp;}
    void SetToLocated(PlanShp plan, VectorSmart sp)
        { assert(plan); cursorState_ = CursorState::LOCATED; plan_ = plan; pos_ = sp;}
    void SetToLocated(PlanShp plan, VectorWorld wp)
        { cursorState_ = CursorState::LOCATED; plan_ = plan; pos_ = plan->GetGrid()->MapWorldtoSmart(wp);}
    //void SetToPort(PlanShp g,

    void SetPosWorld(VectorWorld);

    void Revalidate();
    WirableShp GetWirable();
    PlanShp GetParentPlan();
    
private:
    CursorState cursorState_;
    PlanShp plan_;
    VectorSmart pos_;
    //std::shared_ptr<PortData> port_;

    sf::RectangleShape shape_;
};


struct WiringPair
{
    WiringPair(PlanShp p, WirableShp f, WirableShp t) :plan(p), from(f), to(t) {}
    PlanShp plan;
    WirableShp from;
    WirableShp to;
};

Shp<WiringPair> RetrieveWiringPair(Cursor & cu1, Cursor & cu2);


#endif	/* CURSOR_HPP */

