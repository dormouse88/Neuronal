/* 
 * File:   WireView.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:57
 */

#include "WireView.hpp"
#include "Neuron.hpp" //to get its position
#include <sstream>

const sf::Vector2f WIRE_FROM_OFFSET { GRID_SIZE / 2.f + sf::Vector2f(28.f, 0.f) };
const sf::Vector2f WIRE_TO_OFFSET { GRID_SIZE / 2.f  - sf::Vector2f(28.f, 0.f) };

WireView::WireView(const Wire & wire_p, const ViewResources & vRes_p)
    :wire_m(wire_p)
{
    weightText.setFont(vRes_p.font);
    weightText.setCharacterSize(18);
    weightText.setColor(sf::Color::Red);
}

void WireView::Draw(sf::RenderTarget & rt)
{
    sf::Color colFrom;
    sf::Color colTo;
    if (wire_m.GetFiring()) {
        colFrom = sf::Color(60,0,0);
        colTo = sf::Color(255,50,50);
    }
    else {
        colFrom = sf::Color(40,40,40);
        colTo = sf::Color(210,210,210);
    }
    sf::Vertex line[] =
        {
            sf::Vertex( mapGridToCoords( wire_m.GetFrom().GetPosition() ) + WIRE_FROM_OFFSET, colFrom ),
            sf::Vertex( mapGridToCoords( wire_m.GetTo().GetPosition() ) + WIRE_TO_OFFSET, colTo ),
        };
    rt.draw(line, 2, sf::Lines);

        //I would've used std::to_string() here but for a MinGW bug
        std::ostringstream ss;
        ss << wire_m.GetWeight();
    weightText.setString(ss.str());
    weightText.setPosition( line[1].position - (line[1].position - line[0].position)*0.45f );
    rt.draw(weightText);

}

