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

