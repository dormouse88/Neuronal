/* 
 * File:   ViewResources.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 17:46
 */

#include "ViewResources.hpp"

ViewResources::ViewResources()
{
    font.loadFromFile("resources/sansation.ttf");
}


//Helper Functions
const float SCALE_FACTOR = 75.f;

sf::Vector2i mapCoordsToGrid(const sf::Vector2f & point)
{
    return sf::Vector2i{round(point.x / SCALE_FACTOR), round(point.y / SCALE_FACTOR)};
}

sf::Vector2f mapGridToCoords(const sf::Vector2i & point)
{
    return sf::Vector2f{point.x * SCALE_FACTOR, point.y * SCALE_FACTOR};
}
