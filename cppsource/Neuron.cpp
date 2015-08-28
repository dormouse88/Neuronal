/* 
 * File:   Neuron.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:05
 */

#include "Neuron.hpp"
#include <sstream>

const sf::Vector2f RECTANGLE { 70.f, 40.f };
const sf::Vector2f MAIN_OFFSET { (GRID_SIZE - RECTANGLE)/2.f };
const sf::Vector2f THRESHOLD_OFFSET { MAIN_OFFSET.x + 18.f, MAIN_OFFSET.y + 2.f };

const sf::Vector2f WIRE_IN_OFFSET { MAIN_OFFSET };
const sf::Vector2f WIRE_OUT_OFFSET { MAIN_OFFSET + RECTANGLE };

NeuronView::NeuronView(const Neuron & n)
    :DeviceView(n.GetWorldPos()),
     shape( RECTANGLE )
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    
    thresholdText.setFont(ViewResources::GetInstance().font);
    //thresholdText.setCharacterSize(20);
    thresholdText.setColor(sf::Color::Red);
}

void NeuronView::Draw(sf::RenderTarget & rt, const Neuron & n)
{
    UpdatePos(n.GetWorldPos());
    shape.setPosition( actualPos + MAIN_OFFSET );
    if (n.GetFiring()) shape.setFillColor(sf::Color::Green);
    else shape.setFillColor(sf::Color::Blue);
    rt.draw(shape);

        //I would've used std::to_string() here but for a MinGW bug
        std::ostringstream ss;
        ss << n.GetThreshold();
    thresholdText.setString(ss.str());
    thresholdText.setPosition( actualPos + THRESHOLD_OFFSET );
    rt.draw(thresholdText);
}





//#include "Wire.hpp"
/**
 * CLASS Neuron
 * @param serial_p
 * @param pos_p
 * @param threshold_p
 */
Neuron::Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p)
    :Device(serial_p, pos_p), firing(false), threshold(threshold_p), receivedCharge(0), v(*this)
{}

void Neuron::ReceiveCharge(bool charge, int weight, int slot)
{
    if (charge) receivedCharge += weight;
}

void Neuron::LogicAct()
{
    PushCharge(GetFiring(), 0);
}

void Neuron::LogicCalculate()
{
    if (receivedCharge >= threshold) SetFiring(true);
    else SetFiring(false);
    receivedCharge = 0;
}

sf::Vector2f Neuron::GetWireAttachPos(WireAttachSide was) const
{
    sf::Vector2f wirePos;
    if (was == WireAttachSide::IN) {
        wirePos = GetWorldPos() + WIRE_IN_OFFSET;
    }
    else {
        wirePos = GetWorldPos() + WIRE_OUT_OFFSET;
    }
    return wirePos;
}

void Neuron::Draw(sf::RenderTarget & rt) { v.Draw(rt, *this); }
void Neuron::Handle(int code)
{
    if (code == 1) { ModifyThreshold(1); }
    if (code == 2) { ModifyThreshold(-1); }
}
