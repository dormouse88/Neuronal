/* 
 * File:   Neuron.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 16:05
 */

#include "Neuron.hpp"
#include "ChipPlan.hpp"  //to call SetModified()

const sf::Vector2f RECTANGLE {}; //{ 70.f, 40.f };
const sf::Vector2f RECEIVED_OFFSET { -26.f, -10.f };
const sf::Vector2f THRESHOLD_OFFSET { -7.f, -19.f };

const sf::Vector2f WIRE_IN_OFFSET  { -33.f, 0.f };
const sf::Vector2f WIRE_OUT_OFFSET { +43.f, 0.f }; //RECTANGLE };

//NeuronView::NeuronView(const Neuron & n)
//{}
void Neuron::InitVisuals()
{
    // W width, H height, L length, P protrusion
    const int MAIN_WB = 30; //width back
    const int MAIN_WF = 15; //width forward
    const int MAIN_HH = 20; //half-height
    const int ASS_P = 0;
    const int ASS_H = 6;
    const int ASS_L = 3;
    const int NOSE_P = 8;
    const int NOSE_H = 6;
    const int NOSE_L = 28;
    const int TNOSE_H = NOSE_H;
    const int TNOSE_P = 25;
    fullShape.setPointCount(12);
    fullShape.setPoint(0,  sf::Vector2f(-MAIN_WB-ASS_L    , 0) );
    fullShape.setPoint(1,  sf::Vector2f(-MAIN_WB-ASS_P    , -ASS_H) );
    fullShape.setPoint(2,  sf::Vector2f(-MAIN_WB          , -MAIN_HH) );
    fullShape.setPoint(3,  sf::Vector2f(+MAIN_WF          , -MAIN_HH) );
    fullShape.setPoint(4,  sf::Vector2f(+MAIN_WF+NOSE_P   , -NOSE_H) );
    fullShape.setPoint(5,  sf::Vector2f(+MAIN_WF+TNOSE_P  , -TNOSE_H) );
    fullShape.setPoint(6,  sf::Vector2f(+MAIN_WF+NOSE_L   , 0) );
    fullShape.setPoint(7,  sf::Vector2f(+MAIN_WF+TNOSE_P  , +TNOSE_H) );
    fullShape.setPoint(8,  sf::Vector2f(+MAIN_WF+NOSE_P   , +NOSE_H) );
    fullShape.setPoint(9,  sf::Vector2f(+MAIN_WF          , +MAIN_HH) );
    fullShape.setPoint(10, sf::Vector2f(-MAIN_WB          , +MAIN_HH) );
    fullShape.setPoint(11, sf::Vector2f(-MAIN_WB-ASS_P    , +ASS_H) );
    fullShape.setOutlineColor(sf::Color::White);
    fullShape.setOutlineThickness(-1.2f);

    const int MAIN_HHS = 9; //half-height-smaller
    simpleShape.setPointCount(12);
    simpleShape.setPoint(0,  sf::Vector2f(-MAIN_WB-ASS_L    , 0) );
    simpleShape.setPoint(1,  sf::Vector2f(-MAIN_WB-ASS_P    , -ASS_H) );
    simpleShape.setPoint(2,  sf::Vector2f(-MAIN_WB          , -MAIN_HHS) );
    simpleShape.setPoint(3,  sf::Vector2f(+MAIN_WF          , -MAIN_HHS) );
    simpleShape.setPoint(4,  sf::Vector2f(+MAIN_WF+NOSE_P   , -NOSE_H) );
    simpleShape.setPoint(5,  sf::Vector2f(+MAIN_WF+TNOSE_P  , -TNOSE_H) );
    simpleShape.setPoint(6,  sf::Vector2f(+MAIN_WF+NOSE_L   , 0) );
    simpleShape.setPoint(7,  sf::Vector2f(+MAIN_WF+TNOSE_P  , +TNOSE_H) );
    simpleShape.setPoint(8,  sf::Vector2f(+MAIN_WF+NOSE_P   , +NOSE_H) );
    simpleShape.setPoint(9,  sf::Vector2f(+MAIN_WF          , +MAIN_HHS) );
    simpleShape.setPoint(10, sf::Vector2f(-MAIN_WB          , +MAIN_HHS) );
    simpleShape.setPoint(11, sf::Vector2f(-MAIN_WB-ASS_P    , +ASS_H) );
    simpleShape.setOutlineColor(sf::Color::White);
    simpleShape.setOutlineThickness(-1.2f);
    
    const int BULB_HW = 8;
    const int RIDGE_HW = 3;
    const int BULB_C = +MAIN_WF+NOSE_L - BULB_HW;
    bulbShape.setPointCount(6);
    bulbShape.setPoint(0,  sf::Vector2f{BULB_C-BULB_HW      , 0} );
    bulbShape.setPoint(1,  sf::Vector2f{BULB_C-RIDGE_HW     , -MAIN_HH} );
    bulbShape.setPoint(2,  sf::Vector2f{BULB_C+RIDGE_HW     , -MAIN_HH} );
    bulbShape.setPoint(3,  sf::Vector2f{BULB_C+BULB_HW      , 0} );
    bulbShape.setPoint(4,  sf::Vector2f{BULB_C+RIDGE_HW     , +MAIN_HH} );
    bulbShape.setPoint(5,  sf::Vector2f{BULB_C-RIDGE_HW     , +MAIN_HH} );
    bulbShape.setOutlineColor(sf::Color::White);
    bulbShape.setOutlineThickness(-1.2f);
    
    thresholdText.setFont(ViewResources::GetInstance().font);
    thresholdText.setCharacterSize(30);
    thresholdText.setColor(sf::Color::Green);

    receivedText.setFont(ViewResources::GetInstance().font);
    receivedText.setCharacterSize(15);
    receivedText.setColor(sf::Color{100,100,100} );
}

void Neuron::Draw(sf::RenderTarget & rt) //, const Neuron & n)
{
    const Neuron & n = *this;
    sf::ConvexShape & shape = n.IsSimple() ? simpleShape : fullShape;
    VectorWorld perceivedPos_;
    perceivedPos_ = n.CalculateOffset(RECTANGLE);
    shape.setPosition( perceivedPos_ );
    if (n.GetFiring())
        shape.setFillColor(sf::Color::Red);
    else
        shape.setFillColor(sf::Color(50,50,200) );
    rt.draw(shape);
    
    if (not n.IsSimple()) {
        thresholdText.setString( patch::to_string( n.GetThreshold() ) );
        thresholdText.setPosition( perceivedPos_ + THRESHOLD_OFFSET );
        rt.draw(thresholdText);
    }
    receivedText.setString( patch::to_string( n.GetReceivedCharge() ) );
    receivedText.setPosition( perceivedPos_ + RECEIVED_OFFSET );
    receivedText.setColor( n.IsThresholdMet() ? sf::Color::Red : sf::Color::Black );
    rt.draw(receivedText);

    if (n.HasBulb()) {
        bulbShape.setPosition( perceivedPos_ );
        if (n.GetBulbCharge())
            bulbShape.setFillColor(sf::Color::Red);
        else
            bulbShape.setFillColor(sf::Color(50,50,200) );
        rt.draw(bulbShape);
    }
}





/**
 * CLASS Neuron
 * @param serial_p
 * @param pos_p
 * @param threshold_p
 */
Neuron::Neuron(int serial_p, sf::Vector2i pos_p, int threshold_p, std::shared_ptr<ChipPlan> cont)
    :Device(serial_p, pos_p, cont)
    ,hasBulb_(true)
    ,bulbCharge_(false)
    ,charge_(false)
    ,threshold_(threshold_p)
    ,receivedNum_(0)
    ,receivedSum_(0)
    //,v(*this)
{
    InitVisuals();
}

void Neuron::ReceiveCharge(bool charge, int weight, int slot)
{
    receivedNum_++;
    if (charge) receivedSum_ += weight;
    
    FireIfReady();
}

void Neuron::LogicAct()
{
    if (hasBulb_)
        PushCharge(bulbCharge_, 0);
    if (GetInWiresNum() == 0)
        FireIfReady();  //should do this ONLY if it has no inputs
}

void Neuron::LogicCalculate()
{
    if (hasBulb_)
        bulbCharge_ = charge_;
    receivedNum_ = 0;
    receivedSum_ = 0;
}

void Neuron::FireIfReady()
{
    if (ReceivedAll())
    {
        charge_ = receivedSum_ >= threshold_;
        if (not hasBulb_)
            PushCharge(charge_, 0);
    }
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

//void Neuron::Draw(sf::RenderTarget & rt) { v.Draw(rt, *this); }
void Neuron::Handle(int code)
{
    if (code == 1) { threshold_ += 1; }
    if (code == 2) { threshold_ -= 1;; }
    if (code == 3) { hasBulb_ = not hasBulb_; bulbCharge_ = false; }
    GetContainer()->SetModified();
}
