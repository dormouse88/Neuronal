/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"

Controller::Controller(Model & model_p, View & view_p)
    :theModel(model_p), theView(view_p), mouseCursorSet(false)
{}


bool Controller::HandleInput()
{
    PlanPos pos1 = theView.cursorOne.GetPlanPos();
    PlanPos pos2 = theView.cursorTwo.GetPlanPos();
    
    //the event loop...
    bool quitYet = false;
    sf::Event event;
    while (theView.GetWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            quitYet = true;
        if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            theView.Resize( sf::Vector2f{static_cast<float>(event.size.width), static_cast<float>(event.size.height)} );
        }
        
        //Mouse Events
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
                theView.cursorOne.SetPosWorld(worldPos);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
                theView.cursorTwo.SetPosWorld(worldPos);
            }
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            if (event.mouseWheel.delta > 0)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos(event.mouseWheel.x, event.mouseWheel.y);
                // convert it to world coordinates
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
                theView.CentreOn(worldPos);
            }
            theView.Zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
        }
        
        //Keyboard Events
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::N)
            {
                if (event.key.shift == false) {
                    BlobFactory::AddNeuron(pos1);
                }
                else {
                    BlobFactory::RemoveDevice(pos1);
                }
            }
            if (event.key.code == sf::Keyboard::J)
            {
                BlobFactory::AddJumper(pos1);
            }
            if (event.key.code == sf::Keyboard::H)
            {
                BlobFactory::AddHandle(pos1);
            }
            if (event.key.code == sf::Keyboard::B)
            {
                if (event.key.shift == false) {
                    BlobFactory::AddWire(pos1, pos2);
                }
                else {
                    BlobFactory::RemoveWire(pos1, pos2);
                }
            }
            if (event.key.code == sf::Keyboard::A) {
                ChipPlanFunc::DeviceHandle(pos1, 1);
            }
            if (event.key.code == sf::Keyboard::Z) {
                ChipPlanFunc::DeviceHandle(pos1, 2);
            }
            if (event.key.code == sf::Keyboard::D) {
                ChipPlanFunc::WireHandle(pos1, pos2, 1);
            }
            if (event.key.code == sf::Keyboard::C) {
                ChipPlanFunc::WireHandle(pos1, pos2, 2);
            }
            if (event.key.code == sf::Keyboard::F) {
                ChipPlanFunc::WireHandle(pos1, pos2, 3);
            }
            if (event.key.code == sf::Keyboard::V) {
                ChipPlanFunc::WireHandle(pos1, pos2, 4);
            }
            if (event.key.code == sf::Keyboard::M)
            {
                ChipPlanFunc::SetPosition(pos1, pos2);
            }
            if (event.key.code == sf::Keyboard::Q)
            {
                theModel.SavePlan( pos1 );
            }
            if (event.key.code == sf::Keyboard::W)
            {
                auto wp = theModel.WipePlan(pos1.GetPlan(), event.key.shift and event.key.control);
                if (wp) {
                    theView.cursorOne.SetGridOnly( wp->GetGrid() );
                    theView.cursorTwo.SetGridOnly( wp->GetGrid() );
                }
            }
            if (event.key.code == sf::Keyboard::Dash)
            {
                auto lp = theModel.LoadPlan(pos1.GetPlan()->GetPlanID() - 1, pos1.GetPlan());
                if (lp) {
                    theView.cursorOne.SetGridOnly( lp->GetGrid() );
                    theView.cursorTwo.SetGridOnly( lp->GetGrid() );
                }
            }
            if (event.key.code == sf::Keyboard::Equal)
            {
                auto lp = theModel.LoadPlan(pos1.GetPlan()->GetPlanID() + 1, pos1.GetPlan());
                if (lp) {
                    theView.cursorOne.SetGridOnly( lp->GetGrid() );
                    theView.cursorTwo.SetGridOnly( lp->GetGrid() );
                }
            }
            if (event.key.code == sf::Keyboard::LBracket)
            {
                theView.cursorOne.NullifyLocation();
            }
            if (event.key.code == sf::Keyboard::RBracket)
            {
                theView.cursorTwo.NullifyLocation();
            }
            if (event.key.code == sf::Keyboard::Space)
            {
                theModel.Logic();
            }
            if (event.key.code == sf::Keyboard::Num1)
            {
                theView.SetHighlightingMode(1);
            }
            if (event.key.code == sf::Keyboard::Num2)
            {
                theView.SetHighlightingMode(2);
            }
            if (event.key.code == sf::Keyboard::Num3)
            {
                theView.SetHighlightingMode(3);
            }
        }
    } //(while events)
    
    if ( sf::Mouse::isButtonPressed(sf::Mouse::Middle) )
    {
        sf::Vector2i pixelPos{ sf::Mouse::getPosition(theView.GetWindow()) };
        sf::Vector2f worldPos{ theView.GetWindow().mapPixelToCoords( pixelPos ) };
        if (!mouseCursorSet) {
            mouseCursorSet = true;
        }
        else {
            theView.Pan( (-worldPos + mouseCursorWorldPos) /1.00f );
        }
        sf::Vector2i newPixelPos{ sf::Mouse::getPosition(theView.GetWindow()) };
        mouseCursorWorldPos = theView.GetWindow().mapPixelToCoords( newPixelPos );
    }
    else {
        mouseCursorSet = false;
    }
 
    //Too early here because handles haven't gone out of scope...
//    if (not theView.cursorOne.GetPlanPos().IsValid())
//    {
//        theView.cursorOne.SetGridOnly(theModel.GetBasePlan()->GetGrid());
//    }
//    if (not theView.cursorTwo.GetPlanPos().IsValid())
//    {
//        theView.cursorTwo.SetGridOnly(theView.cursorOne.GetPlanPos().GetGrid());
//    }

    return quitYet;
}

