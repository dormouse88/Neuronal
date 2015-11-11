/* 
 * File:   PlanPos.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 11:52
 */

#include "PlanPos.hpp"

#include "ChipPlan.hpp"

PlanPos::PlanPos(sf::Vector2i newPos, std::shared_ptr<ChipPlan> newPlan)
    :pos(newPos)
    ,plan(newPlan)
{}

PlanPos::PlanPos(sf::Vector2f newPos, std::shared_ptr<ChipPlan> newPlan)
    :pos( newPlan->MapPFtoPI(newPos) )
    ,plan(newPlan)
{}

sf::Vector2i PlanPos::GetPIPos() const
{
    return pos;
}
sf::Vector2f PlanPos::GetPFPos() const
{
    return plan->MapPItoPF(pos);
}
sf::Vector2f PlanPos::GetPFSize() const
{
    return plan->GetPFSize(pos);
}
void PlanPos::SetPlan(std::shared_ptr<ChipPlan> newPlan)
{
    plan = newPlan;
}
void PlanPos::SetPos(sf::Vector2i newPos)
{
    pos = newPos;
}
void PlanPos::SetPos(sf::Vector2f newPos)
{
    pos = plan->MapPFtoPI(newPos);
}
