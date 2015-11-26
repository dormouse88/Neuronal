/* 
 * File:   ViewResources.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 17:46
 */

#ifndef VIEWRESOURCES_HPP
#define	VIEWRESOURCES_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

const sf::Vector2f GRID_SIZE { 115.f, 75.f };

class ViewResources
{
private:
    ViewResources();  //singleton
public:
    ViewResources(const ViewResources&) = delete;

    static const ViewResources & GetInstance();
    sf::Font font;
    sf::Clock clock;
};



#endif	/* VIEWRESOURCES_HPP */

