/* 
 * File:   Cursor.cpp
 * Author: Dormouse
 * 
 * Created on 18 January 2015, 20:37
 */

#include "Cursor.hpp"

Cursor::Cursor(sf::Color color)
{
    representation.setFillColor(sf::Color::Transparent);
    representation.setOutlineColor( color );
    representation.setOutlineThickness(2.5f);
    representation.setSize( sf::Vector2f{GRID_SIZE_X, GRID_SIZE_Y} );
    representation.setPosition( sf::Vector2f{0.f, 0.f} );
}

void Cursor::Draw(sf::RenderTarget & rt)
{
    rt.draw(representation);
}

