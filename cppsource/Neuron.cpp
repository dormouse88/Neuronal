/* 
 * File:   Neuron.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:05
 */

#include "Neuron.hpp"
#include "ChipPlan.hpp"  //to call SetModified()

const sf::Vector2f RECTANGLE { 70.f, 40.f };
const sf::Vector2f RECEIVED_OFFSET { 4.f, 2.f };
const sf::Vector2f THRESHOLD_OFFSET { 24.f, 2.f };

const sf::Vector2f WIRE_IN_OFFSET { };
const sf::Vector2f WIRE_OUT_OFFSET { RECTANGLE };

NeuronView::NeuronView(const Neuron & n)
    :DeviceView(n.GetWorldPos()),
     shape( RECTANGLE )
{
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    
    thresholdText.setFont(ViewResources::GetInstance().font);
    thresholdText.setCharacterSize(30);
    thresholdText.setColor(sf::Color::Green);

    receivedText.setFont(ViewResources::GetInstance().font);
    receivedText.setCharacterSize(15);
    receivedText.setColor(sf::Color{100,100,100} );
}

void NeuronView::Draw(sf::RenderTarget & rt, const Neuron & n)
{
    UpdatePos( n.CalculateOffset(RECTANGLE) );
    shape.setPosition( perceivedPos );
    if (n.GetFiring()) shape.setFillColor(sf::Color::Red);
    else shape.setFillColor(sf::Color(50,50,200) );
    rt.draw(shape);

    thresholdText.setString( patch::to_string( n.GetThreshold() ) );
    thresholdText.setPosition( perceivedPos + THRESHOLD_OFFSET );
    rt.draw(thresholdText);
    receivedText.setString( patch::to_string( n.GetReceivedCharge() ) );
    receivedText.setPosition( perceivedPos + RECEIVED_OFFSET );
    receivedText.setColor( n.IsThresholdMet() ? sf::Color::Red : sf::Color::Black );
    rt.draw(receivedText);
}





//#include "Wire.hpp"
/**
 * CLASS Neuron
 * @param serial_p
 * @param pos_p
 * @param threshold_p
 */
Neuron::Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p, std::shared_ptr<ChipPlan> cont)
    :Device(serial_p, pos_p, cont), firing(false), threshold(threshold_p), receivedCharge(0), v(*this)
{}

void Neuron::ReceiveCharge(bool charge, int weight, int slot)
{
    if (charge) receivedCharge += weight;
}

void Neuron::LogicAct()
{
    PushCharge(firing, 0);
}

void Neuron::LogicCalculate()
{
    firing = IsThresholdMet();
    receivedCharge = 0;
}

sf::Vector2f Neuron::GetWireAttachPos(WireAttachSide was) const
{
    sf::Vector2f wirePos;
    if (was == WireAttachSide::IN) {
        wirePos = CalculateOffset(RECTANGLE) + WIRE_IN_OFFSET;
    }
    else {
        wirePos = CalculateOffset(RECTANGLE) + WIRE_OUT_OFFSET;
    }
    return wirePos;
}

void Neuron::Draw(sf::RenderTarget & rt) { v.Draw(rt, *this); }
void Neuron::Handle(int code)
{
    if (code == 1) { ModifyThreshold(1); }
    if (code == 2) { ModifyThreshold(-1); }
    GetContainer()->SetModified();
}
