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
    sf::Vector2f posFrom = w.GetFrom().GetWireAttachPos(WireAttachSide::OUT, w.GetFromTag());
    line.push_back( sf::Vertex( posFrom, colFrom ) );
    sf::Vector2f posTo = w.GetTo().GetWireAttachPos(WireAttachSide::IN, w.GetToTag());
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
        fromSlotText.setString( patch::to_string(w.GetFromTag()) );
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
        toSlotText.setString( patch::to_string(w.GetToTag()) );
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
 * @param from reference to Wirable object the wire is coming from
 * @param fromSlot
 * @param to reference to Wirable object the wire is going to
 * @param toSlot
 * @param weight
 */
Wire::Wire(Wirable & from, Tag fromTag, Wirable & to, Tag toTag, signed weight, PlanShp cont)
    :PlanOwned(cont)
    , v_(*this)
    , from_(from)
    , fromTag_(fromTag)
    , to_(to)
    , toTag_(toTag)
    , weight_(weight)
    , charge_(Charge::MAYBE)
{}

void Wire::Refresh()
{
    Charge newState = from_.GetOutgoingCharge(fromTag_);
    if (newState != charge_) {
        charge_ = newState;
        to_.ReCalculateCharge(toTag_);
    }
}


void Wire::Draw(sf::RenderTarget & rt)
{
    v_.Draw(rt, *this);
}

void Wire::Handle(HandleCode code)
{
    if (code == 1)
    {
        if (GetFrom().IsSlotted(SlottedSide::OUT)) {
            TagCycle(1, true);
        }
    }
    if (code == 2)
    {
        if (GetFrom().IsSlotted(SlottedSide::OUT)) {
            TagCycle(-1, true);
        }
    }
    if (code == 3)
    {
        if (GetTo().IsSlotted(SlottedSide::IN)) {
            TagCycle(1, false);
        }
        else {
            SetWeight( GetWeight() + 1);
        }
    }
    if (code == 4)
    {
        if (GetTo().IsSlotted(SlottedSide::IN)) {
            TagCycle(-1, false);
        }
        else {
            SetWeight( GetWeight() - 1);
        }
    }
    SetModified();
}

void Wire::SetModified()
{
    GetContainer()->SetModified();    
}

void Wire::SetWeight(CWeight w)
{
    if (weight_ != w)
    {
        weight_ = w;
        to_.ReCalculateCharge(toTag_);
    }
}

void Wire::TagCycle(int step, bool fromSide)
{
//    assert(step == -1 or step == 1);
//    Tag & tagRef = (fromSide == true) ? fromTag_ : toTag_;
//    
//    std::set<Tag> cloud = (fromSide) ? from_.GetTagCloud(InOut::OUT) : to_.GetTagCloud(InOut::IN) ;
//    auto it = cloud.find(tagRef);
//    assert(it != cloud.end());
//    if (it != cloud.begin() and step == -1)
//        it--;
//    else if (step == 1)
//        it++;
//
//    Tag newTag;
//    if (it != cloud.end())
//    {
//        newTag = *it;
//    }
//    else
//    {
//        if (fromSide)
//            newTag = from_.GetFirstFreeTag( InOut::OUT );
//        else
//            newTag = to_.GetFirstFreeTag( InOut::IN );
//    }
//
//    Tag newFrom = (fromSide) ? newTag : fromTag_ ;
//    Tag newTo = (fromSide) ? toTag_ : newTag ;
//    if (from_.CanRegisterExactWire(newFrom, to_, newTo))
//    {
//        Tag oldSlot = toTag_;
//        tagRef = newTag;
//        if (fromSide)
//            Refresh();
//        else
//        {
//            to_.ReCalculateCharge(toTag_);
//            to_.ReCalculateCharge(oldSlot);
//        }
//    }
}



//void Wire::TagCycle(int step, bool fromSide)
//{
//    assert(step == -1 or step == 1);
//    //Tag & chosenSlot = (fromSide == true) ? fromTag_ : toTag_;
//    Tag newFrom = fromTag_;
//    Tag newTo = toTag_;
//    Tag & changingTag = (fromSide == true) ? newFrom : newTo ;
//
//    bool acceptNewSlot = false;
//    while (changingTag + step >= 1 and changingTag + step <= SLOT_MAX and not acceptNewSlot)
//    {
//        changingTag += step;
//        if (from_.CanRegisterExactWire(newFrom, to_, newTo))
//        {
//            acceptNewSlot = true;
//        }
//    }
//    if (acceptNewSlot)
//    {
//        Tag oldSlot = toTag_;
//        (fromSide == true) ? fromTag_ = changingTag : toTag_ = changingTag ;
//        if (fromSide)
//            Refresh();
//        else
//        {
//            to_.ReCalculateCharge(toTag_);
//            to_.ReCalculateCharge(oldSlot);
//        }
//    }
//}
//
