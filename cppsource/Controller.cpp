/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"
#include <cassert>

Controller::Controller(Model & model_p, View & view_p)
    :theModel(model_p), theView(view_p), mouseCursorSet(false), enteringName(false), enteringFilter(false)
{}

void Controller::CursorsGoHere(std::shared_ptr<ChipPlan> p)
{
    if (p) {
        theView.cursorOne.SetGridOnly( p->GetGrid() );
        theView.cursorTwo.SetGridOnly( p->GetGrid() );
        
        auto b = p->GetWorldPaddedBoundBox();  //unimportant
        theView.CentreOn( {b.left + b.width*0.5f, b.top + b.height} );
        theView.Clamp();
    }
}

bool Controller::HandleInput()
{
    //the event loop...
    bool quitYet = false;
    sf::Event event;
    while (theView.GetWindow().pollEvent(event))
    {
        PlanPos pos1 = theView.cursorOne.GetPlanPos();
        PlanPos pos2 = theView.cursorTwo.GetPlanPos();
        
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

        if (enteringName or enteringFilter)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::BackSpace)
                {
                    if (enteringText.length() > 0) enteringText.erase(enteringText.length()-1);
                }
                if (event.key.code == sf::Keyboard::Return or event.key.code == sf::Keyboard::Escape)
                {
                    if (event.key.code == sf::Keyboard::Return)
                    {
                        if (enteringName) {
                            theModel.AddName(pos1.GetPlan()->GetPlanID(), enteringText);
                        }
                        if (enteringFilter) {
                            auto p = theModel.SetNameFilter(pos1, enteringText);
                            CursorsGoHere(p);
                        }
                    }
                    enteringName = false;
                    enteringFilter = false;
                    enteringText.clear();
                }
            }
            if (event.type == sf::Event::TextEntered and textEntryCooldown.getElapsedTime() > sf::milliseconds(200) )
            {
                sf::Uint32 ch = event.text.unicode;
                if (ch >= 0x61 and ch <= 0x7A) {  //convert lowercase to capitals
                    ch -= 0x20;
                }
                if (    (ch >= 0x30 and ch <= 0x39) or  //numbers
                        (ch >= 0x41 and ch <= 0x5A) or  //capitals
                        (ch == 0x23)    )  //#
                {
                    enteringText.append( sf::String{ch} );
                }
            }
        }
        else  //free keyboard response...
        {
            //Keyboard Events
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::N)
                {
                    if (event.key.shift == false) {
                        theModel.GetFactory()->AddNeuron(pos1);
                    }
                    else {
                        theModel.GetFactory()->RemoveDevice(pos1);
                    }
                }
                if (event.key.code == sf::Keyboard::J)
                {
                    ;//theModel.GetFactory()->AddJumper(pos1);
                }
                if (event.key.code == sf::Keyboard::H)
                {
                    theModel.GetFactory()->AddHandle(pos1);
                }
                if (event.key.code == sf::Keyboard::B)
                {
                    if (event.key.shift == false) {
                        theModel.GetFactory()->AddWire(pos1, pos2);
                    }
                    else {
                        theModel.GetFactory()->RemoveWire(pos1, pos2);
                    }
                }
                if (event.key.code == sf::Keyboard::A) {
                    theView.PostMessage("Tried to modify a device upwards");
                    ChipPlanFunc::DeviceHandle(pos1, 1);
                }
                if (event.key.code == sf::Keyboard::Z) {
                    ChipPlanFunc::DeviceHandle(pos1, 2);
                }
                if (event.key.code == sf::Keyboard::BackSlash) {
                    ChipPlanFunc::DeviceHandle(pos1, 3);
                }
                if (event.key.code == sf::Keyboard::S) {
                    ChipPlanFunc::DeviceHandle(pos1, 3);
                }
                if (event.key.code == sf::Keyboard::X) {
                    ChipPlanFunc::DeviceHandle(pos1, 4);
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
                    theView.PostMessage("Tried to move something");
                    ChipPlanFunc::SetPosition(pos1, pos2);
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    if (event.key.shift) theModel.SavePlanAsNew( pos1 );
                    else theModel.SavePlan( pos1 );
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    auto p = theModel.WipePlan(pos1, event.key.shift and event.key.control);
                    CursorsGoHere(p);
                }
                
                if (event.key.code == sf::Keyboard::Numpad1)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::PREV_ID);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad3)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::NEXT_ID);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad7)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::PREV_NAME);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad9)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::NEXT_NAME);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad8)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::PARENT);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad2)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::FIRST_CHILD);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad4)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::PREV_SIBLING);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Numpad6)
                {
                    auto p = theModel.LoadPlan(pos1, PlanNav::NEXT_SIBLING);
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Subtract)
                {
                    assert(not enteringName);
                    assert(not enteringFilter);
                    auto p = theModel.SetNameFilter(pos1, "");
                    CursorsGoHere(p);
                }
                if (event.key.code == sf::Keyboard::Add)
                {
                    assert(not enteringName);
                    assert(not enteringFilter);
                    enteringFilter = true;
                    enteringText = theModel.GetNameFilter();
                    textEntryCooldown.restart();
                }

                if (event.key.code == sf::Keyboard::R)
                {
                    theModel.RemoveName(pos1.GetPlan()->GetPlanID());
                }
                if (event.key.code == sf::Keyboard::E and theModel.CanAddName(pos1.GetPlan()->GetPlanID()))
                {
                    assert(not enteringName);
                    assert(not enteringFilter);
                    enteringName = true;
                    textEntryCooldown.restart();
                }

                if (event.key.code == sf::Keyboard::LBracket)
                {
                    theView.cursorOne.Dislocate();
                }
                if (event.key.code == sf::Keyboard::RBracket)
                {
                    theView.cursorTwo.Dislocate();
                }
                if (event.key.code == sf::Keyboard::Comma)
                {
                    theModel.InnerTick();
                }
                if (event.key.code == sf::Keyboard::Period)
                {
                    theModel.OuterTick();
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
            } //(if event.type is keyPressed)
        }//else (not entering text)
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

    theView.SetTextEntering(enteringName or enteringFilter, enteringText);
    
    return quitYet;
}

