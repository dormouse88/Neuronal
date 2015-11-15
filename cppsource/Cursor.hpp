/* 
 * File:   Cursor.hpp
 * Author: Dormouse
 *
 * Created on 18 January 2015, 20:37
 */

#ifndef CURSOR_HPP
#define	CURSOR_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
class ChipPlan;
#include "PlanPos.hpp"

class Cursor
{
public:
    Cursor(sf::Color color = sf::Color::Yellow);
    void Draw(sf::RenderTarget & rt);

    std::shared_ptr<const PlanPos> GetPlanPos() const         {return pposPtr;}
    void SetPlanPos(std::shared_ptr<PlanPos> ppos)            {pposPtr = ppos;}

    void SetPosSmart(VectorSmart, std::shared_ptr<PlanGrid>);
    void SetPosWorld(VectorWorld, std::shared_ptr<PlanGrid>);
    void Nullify()                                            {pposPtr = nullptr;}

private:
    std::shared_ptr<PlanPos> pposPtr;
    sf::RectangleShape representation;
};

#endif	/* CURSOR_HPP */

