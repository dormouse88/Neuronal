/* 
 * File:   Cursor.cpp
 * Author: Dormouse
 * 
 * Created on 18 January 2015, 20:37
 */

#include "Cursor.hpp"

Cursor::Cursor(sf::Color color)
    :active(false), m_pos(sf::Vector2i{0, 0})
{
    representation.setFillColor(sf::Color::Transparent);
    representation.setOutlineColor( color );
    representation.setOutlineThickness(2.5f);
    representation.setSize( GRID_SIZE );
}

void Cursor::Draw(sf::RenderTarget & rt)
{
    if (active) {
        representation.setPosition( mapGridToCoords(m_pos) );
        rt.draw(representation, sf::RenderStates(sf::BlendAdd));
    }
}

