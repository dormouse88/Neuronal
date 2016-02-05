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
#include "BasicTypes.hpp"
#include "ViewResources.hpp"
#include "ChipPlan.hpp"  //class PlanGrid;
#include "PlanPos.hpp"


enum class CursorState { ABSENT, PLAN, LOCATED, PORT };

class Cursor
{
public:
    Cursor(std::shared_ptr<PlanGrid> g, sf::Color color);
    void Draw(sf::RenderTarget & rt);

    CursorState GetState() const                        { return cursorState_; }
    PlanShp GetPlan()                                   { assert(cursorState_ != CursorState::ABSENT); return plan_; }
    PlanPos GetPlanPos()                                { assert(cursorState_ == CursorState::LOCATED); return PlanPos(pos_, plan_->GetGrid()); }
    PortLocation GetPort()                              { assert(cursorState_ == CursorState::PORT); return port_; }
    
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
    void SetToPort(PortLocation pl)
        { cursorState_ = CursorState::PORT; port_ = pl;}

    void SetPosWorld(VectorWorld);

    void Revalidate();
    WirableShp GetWirable();
    PlanShp GetParentPlan();
    
private:
    CursorState cursorState_;
    PlanShp plan_;
    VectorSmart pos_;
    PortLocation port_;

    sf::RectangleShape shape_;
};


Shp<WiringPair> RetrieveWiringPair(Cursor & cu1, Cursor & cu2);


#endif	/* CURSOR_HPP */

