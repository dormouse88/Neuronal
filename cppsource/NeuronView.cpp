/* 
 * File:   NeuronView.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:57
 */

#include "NeuronView.hpp"
#include <string>
#include <sstream>
#include <iostream>

const float SIZE_X = 70.f;
const float SIZE_Y = 40.f;

NeuronView::NeuronView(const Neuron & neuron_p, const ViewResources & vRes_p)
    :shape(sf::Vector2f(SIZE_X, SIZE_Y)),
     neuron_m(neuron_p)
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setPosition(mapGridToCoords(neuron_m.GetPosition()) );
    
    thresholdText.setFont(vRes_p.font);
    //thresholdText.setCharacterSize(20);
    thresholdText.setColor(sf::Color::Red);
    thresholdText.setPosition(mapGridToCoords(neuron_m.GetPosition()) + sf::Vector2f(18.f, 2.f) );
}


void NeuronView::Draw(sf::RenderTarget & rt)
{
    if (neuron_m.firing) shape.setFillColor(sf::Color::Green);
    else shape.setFillColor(sf::Color::Blue);
    rt.draw(shape);

        //I would've used std::to_string() here but for a MinGW bug
        std::ostringstream ss;
        ss << neuron_m.GetThreshold();
    thresholdText.setString(ss.str());
    rt.draw(thresholdText);
}
