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
class PlanGrid;
#include "PlanPos.hpp"

class Cursor
{
public:
    Cursor(std::shared_ptr<PlanGrid> g, sf::Color color = sf::Color::Yellow);
    void Draw(sf::RenderTarget & rt);

    PlanPos GetPlanPos() const;
    void SetGridOnly(std::shared_ptr<PlanGrid> g)               { ppos.SetGrid(g); }
    void NullifyLocation()                                      { ppos.SetPlanOnly(); }
    void SetPosWorld(VectorWorld);
    
private:
    PlanPos ppos;
    sf::RectangleShape representation;
};


#endif	/* CURSOR_HPP */

