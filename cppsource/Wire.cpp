/* 
 * File:   wire.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:09
 */

#include "Wire.hpp"
#include "Neuron.hpp" //to get its position

Wire::Wire(Neuron & from_p, Neuron & to_p)
    :from(from_p), to(to_p), weight(1), firing(false)
{
    from.RegisterOut(this);
    to.RegisterIn(this);
}

void Wire::Draw(sf::RenderTarget & rt)
{
    sf::Color col;
    if (firing) col = sf::Color(255,50,50);
    else col = sf::Color(170,170,170);
    sf::Vertex line[] =
        {
            //two methods... (both work)
            sf::Vertex(static_cast<sf::Vector2f>(from.GetPosition()), col),
            sf::Vertex(sf::Vector2f(to.GetPosition().x - 5.f, to.GetPosition().y), col)
        };
    rt.draw(line, 2, sf::Lines);
}
