/* 
 * File:   Neuron.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:05
 */

#include "Neuron.hpp"
#include "Wire.hpp"
#include <string>
#include <sstream>
#include <iostream>

const float SIZE_X = 70.f;
const float SIZE_Y = 40.f;
const int INITIAL_THRESHOLD = 1;

Neuron::Neuron(int x, int y, const sf::Font & font_p)
    :shape(sf::Vector2f(SIZE_X, SIZE_Y)), threshold(INITIAL_THRESHOLD), pos(x, y), firing(false)
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setPosition(pos.x, pos.y);
    
    thresholdText.setFont(font_p);
    //thresholdText.setCharacterSize(20);
    thresholdText.setColor(sf::Color::Red);
    thresholdText.setPosition(pos.x+18, pos.y+2);
}


void Neuron::StepPartOne()
{
    //get inputs and calculate firing bool
    int total = 0;
    for (const auto & w : inWires)
    {
        if (w->GetFiring()) total += w->GetWeight();
    }
    if (total >= threshold) firing = true;
    else firing = false;
}

void Neuron::StepPartTwo()
{
    //set outputs
    for (auto & w : outWires)
    {
        w->SetFiring(firing);
    }
}

void Neuron::Draw(sf::RenderTarget & rt)
{
    if (firing) shape.setFillColor(sf::Color::Green);
    else shape.setFillColor(sf::Color::Blue);
    rt.draw(shape);

        //I would've used std::to_string() here but for a MinGW bug
        std::ostringstream ss;
        ss << threshold;
    thresholdText.setString(ss.str());
    rt.draw(thresholdText);
}