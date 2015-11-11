/* 
 * File:   PlanPos.hpp
 * Author: Dormouse
 *
 * Created on 11 November 2015, 11:52
 */

#ifndef PLANPOS_HPP
#define	PLANPOS_HPP

#include <memory>
#include <SFML/System.hpp>  //for sf::Vector
class ChipPlan;

class PlanPos
{
public:
    PlanPos(sf::Vector2i newPos, std::shared_ptr<ChipPlan> newPlan);
    PlanPos(sf::Vector2f newPos, std::shared_ptr<ChipPlan> newPlan);
    sf::Vector2i GetPIPos() const;
    sf::Vector2f GetPFPos() const;
    sf::Vector2f GetPFSize() const;
    void SetPlan(std::shared_ptr<ChipPlan> newPlan);
    void SetPos(sf::Vector2i newPos);
    void SetPos(sf::Vector2f newPos);
private:
    sf::Vector2i pos;
    std::shared_ptr<ChipPlan> plan;
};

#endif	/* PLANPOS_HPP */

