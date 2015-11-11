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
    //std::shared_ptr<sf::Vector2f> GetPFPos() const            {if (pposPtr) return std::make_shared<sf::Vector2f>(pposPtr->GetPFPos()); else return nullptr;}
    std::shared_ptr<sf::Vector2i> GetPIPos() const            {if (pposPtr) return std::make_shared<sf::Vector2i>(pposPtr->GetPIPos()); else return nullptr;}
    void SetPFPos(sf::Vector2f newPos, std::shared_ptr<ChipPlan> newPlan);
    void Nullify()                                            {pposPtr = nullptr;}

private:
    std::shared_ptr<PlanPos> pposPtr;
    sf::RectangleShape representation;
};

#endif	/* CURSOR_HPP */

