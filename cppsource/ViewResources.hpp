/* 
 * File:   ViewResources.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 17:46
 */

#ifndef VIEWRESOURCES_HPP
#define	VIEWRESOURCES_HPP

#include <SFML/Graphics.hpp>

const sf::Vector2f GRID_SIZE { 115.f, 75.f };

class ViewResources
{
public:
    ViewResources();
    ViewResources(const ViewResources&) = delete;

    sf::Font font;
};



sf::Vector2i mapCoordsToGrid(const sf::Vector2f & point);
sf::Vector2f mapGridToCoords(const sf::Vector2i & point);



#endif	/* VIEWRESOURCES_HPP */

