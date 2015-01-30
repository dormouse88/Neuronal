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
sf::Vector2i mapCoordsToGrid(const sf::Vector2f & point)
{
    return sf::Vector2i{floor(point.x / GRID_SIZE.x), floor(point.y / GRID_SIZE.y)};
}

sf::Vector2f mapGridToCoords(const sf::Vector2i & point)
{
    return sf::Vector2f{point.x * GRID_SIZE.x, point.y * GRID_SIZE.y};
}
