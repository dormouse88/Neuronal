/* 
 * File:   Cursor.hpp
 * Author: Dormouse
 *
 * Created on 18 January 2015, 20:37
 */

#ifndef CURSOR_HPP
#define	CURSOR_HPP

#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"

class Cursor
{
public:
    Cursor(sf::Color color = sf::Color::Yellow);
    void Draw(sf::RenderTarget & rt);

    sf::Vector2f GetWorldPos() const { if (!active) throw; return mapGridToCoords(m_pos); }
    sf::Vector2i GetGridPos() const { if (!active) throw; return m_pos; }
    void SetWorldPos(sf::Vector2f worldPos) { m_pos = mapCoordsToGrid(worldPos); active=true;}
    void SetGridPos(sf::Vector2i gridPos) { m_pos = gridPos; active=true;}

private:
    sf::Vector2i m_pos;
    sf::RectangleShape representation;
public:
    bool active;
};

#endif	/* CURSOR_HPP */

