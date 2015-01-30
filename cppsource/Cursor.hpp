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
//#include "Neuron.cpp"//

class Cursor
{
public:
    Cursor(sf::Color color = sf::Color::Yellow);
    void Draw(sf::RenderTarget & rt);
    sf::Vector2f GetPos() const { return representation.getPosition(); }
    void SetPos(sf::Vector2f p) { representation.setPosition(p); }
private:
    sf::RectangleShape representation;
    //Neuron * neuron;//
};

#endif	/* CURSOR_HPP */

