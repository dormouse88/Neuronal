/* 
 * File:   wire.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:09
 */

#include "Wire.hpp"

#include "Neuron.hpp" //to get its position #include "Device.hpp" //to get its position
#include <sstream>
//#include <vector>
//#include <memory>

const sf::Vector2f WIRE_FROM_OFFSET  { GRID_SIZE / 2.f + sf::Vector2f(+28.f, +8.f) };
const sf::Vector2f WIRE_LOOP1_OFFSET { GRID_SIZE / 2.f + sf::Vector2f(+20.f, -30.f) };
const sf::Vector2f WIRE_LOOP2_OFFSET { GRID_SIZE / 2.f + sf::Vector2f(-30.f, -30.f) };
const sf::Vector2f WIRE_TO_OFFSET    { GRID_SIZE / 2.f + sf::Vector2f(-28.f, -8.f) };

WireView::WireView(const Wire & wire_p)
{
    weightText.setFont(ViewResources::GetInstance().font);
    weightText.setCharacterSize(18);
    weightText.setColor(sf::Color::Red);

    fromSlotText.setFont(ViewResources::GetInstance().font);
    fromSlotText.setCharacterSize(18);
    fromSlotText.setColor(sf::Color::Cyan);

    toSlotText.setFont(ViewResources::GetInstance().font);
    toSlotText.setCharacterSize(18);
    toSlotText.setColor(sf::Color::Magenta);
}

void WireView::Draw(sf::RenderTarget & rt, const Wire & w)
{
    //The Line...
    sf::Color colFrom;
    sf::Color colTo;
    if (w.GetFiring()) {
        colFrom = sf::Color(60,0,0);
        colTo = sf::Color(255,50,50);
    }
    else {
        colFrom = sf::Color(40,40,40);
        colTo = sf::Color(210,210,210);
    }
    std::vector<sf::Vertex> line;
    line.push_back( sf::Vertex(  w.GetFrom().GetWireAttachPos(WireAttachSide::OUT), colFrom ) );
    if ( &w.GetFrom() == &w.GetTo())
    {
        line.push_back( sf::Vertex( w.GetFrom().GetWireAttachPos(WireAttachSide::OUT) + WIRE_LOOP1_OFFSET, colTo ) ); //WireAttachSide::OUT is a temporary hack
        line.push_back( sf::Vertex( w.GetTo().GetWireAttachPos(WireAttachSide::OUT)   + WIRE_LOOP2_OFFSET, colTo ) );
    }
    line.push_back( sf::Vertex( w.GetTo().GetWireAttachPos(WireAttachSide::IN), colTo ) );
    rt.draw(&line[0], line.size(), sf::LinesStrip);

    //The Text...
    //(I would've used std::to_string() here rather than a stringstream but for a MinGW bug)
    if (w.GetTo().IsWeightedIn()) {
        std::ostringstream ss;
        ss << w.GetWeight();
        weightText.setString(ss.str());
        weightText.setPosition( line[1].position - (line[1].position - line[0].position)*0.45f );
        rt.draw(weightText);
    }

    if (w.GetFrom().IsSlotted(SlottedSide::OUT)) {
        std::ostringstream ss;
        ss << w.GetFromSlot();
        fromSlotText.setString(ss.str());
        fromSlotText.setPosition( line[1].position - (line[1].position - line[0].position)*0.75f );
        rt.draw(fromSlotText);
    }

    if (w.GetTo().IsSlotted(SlottedSide::IN)) {
        std::ostringstream ss;
        ss << w.GetToSlot();
        toSlotText.setString(ss.str());
        toSlotText.setPosition( line[1].position - (line[1].position - line[0].position)*0.25f );
        rt.draw(toSlotText);
    }
}



/**
 * CLASS Wire
 * @param from_p reference to Wirable object the wire is coming from
 * @param fromSlot_p
 * @param to_p reference to Wirable object the wire is going to
 * @param toSlot_p
 * @param weight_p
 */
Wire::Wire(Wirable & from_p, int fromSlot_p, Wirable & to_p, int toSlot_p, signed weight_p)
    :v(*this), from(from_p), fromSlot(fromSlot_p), to(to_p), toSlot(toSlot_p), weight(weight_p), firing(false)
{}

void Wire::ReceiveCharge(bool f)
{
    firing = f;
    to.ReceiveCharge(firing, weight, toSlot);
}

void Wire::Draw(sf::RenderTarget & rt) { v.Draw(rt, *this); }
void Wire::Handle(int code)
{
    if (code == 1) { SetWeight( GetWeight() + 1); }
    if (code == 2) { SetWeight( GetWeight() - 1); }
}
