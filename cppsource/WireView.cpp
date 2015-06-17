/* 
 * File:   WireView.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:57
 */

#include "WireView.hpp"
#include "Neuron.hpp" //to get its position
#include <sstream>
#include <vector>
#include <memory>

const sf::Vector2f WIRE_FROM_OFFSET  { GRID_SIZE / 2.f + sf::Vector2f(+28.f, +8.f) };
const sf::Vector2f WIRE_LOOP1_OFFSET { GRID_SIZE / 2.f + sf::Vector2f(+20.f, -30.f) };
const sf::Vector2f WIRE_LOOP2_OFFSET { GRID_SIZE / 2.f + sf::Vector2f(-30.f, -30.f) };
const sf::Vector2f WIRE_TO_OFFSET    { GRID_SIZE / 2.f + sf::Vector2f(-28.f, -8.f) };

WireView::WireView(std::shared_ptr<const Wire> wire_p, const ViewResources & vRes_p)
    :GobjectView(wire_p), wire_m(wire_p)
{
    weightText.setFont(vRes_p.font);
    weightText.setCharacterSize(18);
    weightText.setColor(sf::Color::Red);
}

void WireView::Draw(sf::RenderTarget & rt)
{
    std::shared_ptr<const Wire> m { wire_m.lock() };
    if (m) {
        sf::Color colFrom;
        sf::Color colTo;
        if (m->GetFiring()) {
            colFrom = sf::Color(60,0,0);
            colTo = sf::Color(255,50,50);
        }
        else {
            colFrom = sf::Color(40,40,40);
            colTo = sf::Color(210,210,210);
        }
        std::vector<sf::Vertex> line;
        line.push_back( sf::Vertex( mapGridToCoords( m->GetFrom().GetPosition() ) + WIRE_FROM_OFFSET, colFrom ) );
        if (m->GetFrom() == m->GetTo())
        {
            line.push_back( sf::Vertex( mapGridToCoords( m->GetFrom().GetPosition() ) + WIRE_LOOP1_OFFSET, colTo ) );
            line.push_back( sf::Vertex( mapGridToCoords( m->GetTo().GetPosition() )   + WIRE_LOOP2_OFFSET, colTo ) );
        }
        line.push_back( sf::Vertex( mapGridToCoords( m->GetTo().GetPosition() ) + WIRE_TO_OFFSET, colTo ) );
        rt.draw(&line[0], line.size(), sf::LinesStrip);

            //I would've used std::to_string() here but for a MinGW bug
            std::ostringstream ss;
            ss << m->GetWeight();
        weightText.setString(ss.str());
        weightText.setPosition( line[1].position - (line[1].position - line[0].position)*0.45f );
        rt.draw(weightText);
    }
}

