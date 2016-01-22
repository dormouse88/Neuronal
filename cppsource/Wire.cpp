/* 
 * File:   wire.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:09
 */

#include "Wire.hpp"
#include "ChipPlan.hpp"  //to call SetModified()
#include "Wirable.hpp"  //fwd dec  //to use all the functions via 'to' and 'from'
#include <cassert>

const float BACKBOARD_PADDING = 4.f;
const sf::Color BACKBOARD_FILLCOLOR {20,20,20};

WireView::WireView(const Wire & wire_p)
{
    weightText.setFont(ViewResources::GetInstance().font);
    weightText.setCharacterSize(18);
    weightText.setColor(sf::Color::Red);
    weightTextBoard.setFillColor( BACKBOARD_FILLCOLOR );
    weightTextBoard.setOutlineColor( sf::Color(80,20,20) );
    weightTextBoard.setOutlineThickness( 1.f );

    fromSlotText.setFont(ViewResources::GetInstance().font);
    fromSlotText.setCharacterSize(18);
    fromSlotText.setColor(sf::Color::Cyan);
    fromSlotTextBoard.setFillColor( BACKBOARD_FILLCOLOR );
    fromSlotTextBoard.setOutlineColor( sf::Color(20,60,60) );
    fromSlotTextBoard.setOutlineThickness( 1.f );
    
    toSlotText.setFont(ViewResources::GetInstance().font);
    toSlotText.setCharacterSize(18);
    toSlotText.setColor(sf::Color::Magenta);
    toSlotTextBoard.setFillColor( BACKBOARD_FILLCOLOR );    
    toSlotTextBoard.setOutlineColor( sf::Color(60,60,20) );
    toSlotTextBoard.setOutlineThickness( 1.f );
}

void WireView::Draw(sf::RenderTarget & rt, const Wire & w)
{
    //The Line...
    sf::Color colFrom;
    sf::Color colTo;
    if (w.GetFiring()) {
        colFrom = sf::Color(80,20,20);     //(60,0,0);
        colTo = sf::Color(225,50,50);   //(255,50,50);
    }
    else {
        colFrom = sf::Color(70,70,70);  //(40,40,40);
        colTo = sf::Color(180,180,180);   //(210,210,210);
    }
    std::vector<sf::Vertex> line;
    sf::Vector2f posFrom = w.GetFrom().GetWireAttachPos(WireAttachSide::OUT);
    line.push_back( sf::Vertex( posFrom, colFrom ) );
    sf::Vector2f posTo = w.GetTo().GetWireAttachPos(WireAttachSide::IN);
    line.push_back( sf::Vertex(posTo, colTo ) );
    rt.draw(&line[0], line.size(), sf::LinesStrip);

    //The pulse...
    if (line.size() == 2) {
        std::vector<sf::Vertex> pulse;
        sf::Time t = ViewResources::GetInstance().clock.getElapsedTime();
        //fmod(t*x, y) + z //pulse every x seconds, along y% of the line. z is length of the pulse as line %.
        float linePoint1 = fmod(t.asSeconds() *1.0, 0.9f) + 0.1f ;
        float linePoint2 = fmod(t.asSeconds() *1.0, 0.9f) ;
        pulse.push_back( sf::Vertex( posFrom + linePoint1 * (posTo - posFrom), sf::Color(175,175,175) * colTo ) );
        pulse.push_back( sf::Vertex( posFrom + linePoint2 * (posTo - posFrom), sf::Color(0,0,0) ) );
        rt.draw(&pulse[0], pulse.size(), sf::LinesStrip, sf::RenderStates(sf::BlendAdd));
    }

    //The Text...
    if (w.GetTo().IsWeightedIn()) {
        //align text on wire...
        weightText.setString( patch::to_string(w.GetWeight()) );
        sf::Vector2f pointOnWire { line[1].position - (line[1].position - line[0].position)*0.45f };
        sf::FloatRect loc{ weightText.getLocalBounds() };
        sf::Vector2f textCentreOffset { -loc.width/2.f, -loc.top-loc.height/2.f };
        weightText.setPosition( pointOnWire + textCentreOffset );
        
        //backboard...
        sf::FloatRect glob { weightText.getGlobalBounds() };
        weightTextBoard.setSize( sf::Vector2f{ glob.width + 2*BACKBOARD_PADDING, glob.height + 2*BACKBOARD_PADDING} ) ;
        weightTextBoard.setPosition( glob.left -BACKBOARD_PADDING, glob.top -BACKBOARD_PADDING) ;

        rt.draw(weightTextBoard);
        rt.draw(weightText);
    }

    if (w.GetFrom().IsSlotted(SlottedSide::OUT)) {
        //align text on wire...
        fromSlotText.setString( patch::to_string(w.GetFromSlot()) );
        sf::Vector2f pointOnWire { line[1].position - (line[1].position - line[0].position)*0.75f };
        sf::FloatRect loc{ fromSlotText.getLocalBounds() };
        sf::Vector2f textCentreOffset { -loc.width/2.f, -loc.top-loc.height/2.f };
        fromSlotText.setPosition( pointOnWire + textCentreOffset );

        //backboard...
        sf::FloatRect glob { fromSlotText.getGlobalBounds() };
        fromSlotTextBoard.setSize( sf::Vector2f{ glob.width + 2*BACKBOARD_PADDING, glob.height + 2*BACKBOARD_PADDING} ) ;
        fromSlotTextBoard.setPosition( glob.left -BACKBOARD_PADDING, glob.top -BACKBOARD_PADDING) ;

        rt.draw(fromSlotTextBoard);
        rt.draw(fromSlotText);
    }

    if (w.GetTo().IsSlotted(SlottedSide::IN)) {
        //align text on wire...
        toSlotText.setString( patch::to_string(w.GetToSlot()) );
        sf::Vector2f pointOnWire { line[1].position - (line[1].position - line[0].position)*0.25f };
        sf::FloatRect loc{ toSlotText.getLocalBounds() };
        sf::Vector2f textCentreOffset { -loc.width/2.f, -loc.top-loc.height/2.f };
        toSlotText.setPosition( pointOnWire + textCentreOffset );

        //backboard...
        sf::FloatRect glob { toSlotText.getGlobalBounds() };
        toSlotTextBoard.setSize( sf::Vector2f{ glob.width + 2*BACKBOARD_PADDING, glob.height + 2*BACKBOARD_PADDING} ) ;
        toSlotTextBoard.setPosition( glob.left -BACKBOARD_PADDING, glob.top -BACKBOARD_PADDING) ;

        rt.draw(toSlotTextBoard);
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
Wire::Wire(Wirable & from_p, int fromSlot_p, Wirable & to_p, int toSlot_p, signed weight_p, PlanShp cont)
    :PlanOwned(cont), v(*this), from(from_p), fromSlot(fromSlot_p), to(to_p), toSlot(toSlot_p), weight(weight_p), firing(false)
{}

void Wire::Refresh()
{
    bool newState = from.GetOutgoingCharge(fromSlot);
    if (newState != firing) {
        firing = newState;
        to.Refresh(toSlot);
    }
}


void Wire::Draw(sf::RenderTarget & rt)
{
    v.Draw(rt, *this);
}

void Wire::Handle(int code)
{
    if (code == 1)
    {
        if (GetFrom().IsSlotted(SlottedSide::OUT)) {
            SlotCycle(1, true);
        }
    }
    if (code == 2)
    {
        if (GetFrom().IsSlotted(SlottedSide::OUT)) {
            SlotCycle(-1, true);
        }
    }
    if (code == 3)
    {
        if (GetTo().IsSlotted(SlottedSide::IN)) {
            SlotCycle(1, false);
        }
        else {
            SetWeight( GetWeight() + 1);
        }
    }
    if (code == 4)
    {
        if (GetTo().IsSlotted(SlottedSide::IN)) {
            SlotCycle(-1, false);
        }
        else {
            SetWeight( GetWeight() - 1);
        }
    }
    GetContainer()->SetModified();
}

void Wire::SlotCycle(int step, bool fromSide)
{
    assert(step == -1 or step == 1);
    int & chosenSlot = fromSide == true ? fromSlot : toSlot;
    int newSlot = chosenSlot;
    bool acceptNewSlot = false;
    while (newSlot + step >= 1 and newSlot + step <= SLOT_MAX and not acceptNewSlot)
    {
        newSlot += step;
        if ( (fromSide and from.CanRegisterOut(newSlot)) or (!fromSide and to.CanRegisterIn(newSlot)) )
        {
            acceptNewSlot = true;
        }
    }
    if (acceptNewSlot) chosenSlot = newSlot;
}