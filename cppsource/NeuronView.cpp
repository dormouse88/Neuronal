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

const sf::Vector2f NEURON_SIZE { 70.f, 40.f };
const sf::Vector2f NEURON_OFFSET { (GRID_SIZE - NEURON_SIZE)/2.f };
const sf::Vector2f THRESHOLD_OFFSET { NEURON_OFFSET.x + 18.f, NEURON_OFFSET.y + 2.f };


NeuronView::NeuronView(const Neuron & neuron_p, const ViewResources & vRes_p)
    :DeviceView(neuron_p),
     neuron_m(neuron_p),
     shape( NEURON_SIZE )
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    
    thresholdText.setFont(vRes_p.font);
    //thresholdText.setCharacterSize(20);
    thresholdText.setColor(sf::Color::Red);
}


void NeuronView::Draw(sf::RenderTarget & rt)
{
    UpdatePos();
    shape.setPosition( actualPos + NEURON_OFFSET );
    if (neuron_m.GetFiring()) shape.setFillColor(sf::Color::Green);
    else shape.setFillColor(sf::Color::Blue);
    rt.draw(shape);

        //I would've used std::to_string() here but for a MinGW bug
        std::ostringstream ss;
        ss << neuron_m.GetThreshold();
    thresholdText.setString(ss.str());
    thresholdText.setPosition( actualPos + THRESHOLD_OFFSET );
    rt.draw(thresholdText);
}
