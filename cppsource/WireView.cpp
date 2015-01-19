/* 
 * File:   WireView.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:57
 */

#include "WireView.hpp"
#include "Neuron.hpp" //to get its position

WireView::WireView(const Wire & wire_p, const ViewResources & vRes_p)
    :wire_m(wire_p)
{}

void WireView::Draw(sf::RenderTarget & rt)
{
    sf::Color col;
    if (wire_m.GetFiring()) col = sf::Color(255,50,50);
    else col = sf::Color(170,170,170);
    sf::Vertex line[] =
        {
            sf::Vertex( mapGridToCoords( wire_m.GetFrom().GetPosition() ), col ),
            sf::Vertex( mapGridToCoords( wire_m.GetTo().GetPosition() ), col ),
        };
    rt.draw(line, 2, sf::Lines);
}

