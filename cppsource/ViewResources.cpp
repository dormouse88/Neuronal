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
    return sf::Vector2i{round(point.x / GRID_SIZE_X), round(point.y / GRID_SIZE_Y)};
}

sf::Vector2f mapGridToCoords(const sf::Vector2i & point)
{
    return sf::Vector2f{point.x * GRID_SIZE_X, point.y * GRID_SIZE_Y};
}
