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
#include "ChipPlan.hpp"

class Cursor
{
public:
    Cursor(sf::Color color = sf::Color::Yellow);
    void Draw(sf::RenderTarget & rt);

    sf::Vector2f GetWorldPos() const;
    sf::Vector2i GetGridPos() const;
    void SetWorldPos(sf::Vector2f worldPos, std::shared_ptr<ChipPlan> plan);
    //Unused// void SetGridPos(sf::Vector2i gridPos) { m_pos = gridPos; active=true;}

private:
    sf::Vector2i m_pos;
    std::weak_ptr<ChipPlan> m_plan;
    sf::RectangleShape representation;
public:
    bool active;
};

#endif	/* CURSOR_HPP */

