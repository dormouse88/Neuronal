/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"
#include <cassert>

Controller::Controller(Model & model_p, View & view_p)
    :model_(model_p), view_(view_p), quitYet_(false), isMouseCursorSet_(false), isEnteringName_(false), isEnteringFilter_(false), cu1(view_.cursorOne), cu2(view_.cursorTwo)
{}



void Controller::CursorsGoHere(PlanShp p)
{
    if (p) {
        view_.cursorOne.SetToPlan( p );
        view_.cursorTwo.SetToPlan( p );
        
        auto b = p->GetWorldPaddedBoundBox();  //unimportant
        view_.CentreOn( {b.left + b.width*0.5f, b.top + b.height} );
        view_.Clamp();
    }
}



bool Controller::HandleInput()
{
    HandleInputEvents();
    HandleInputState();
    return quitYet_;
}



void Controller::HandleInputEvents()
{
    //sf::Event event;
    while (view_.GetWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            quitYet_ = true;
        if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            view_.Resize( sf::Vector2f{static_cast<float>(event.size.width), static_cast<float>(event.size.height)} );
        }
        
        //Mouse Events
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = view_.GetWindow().mapPixelToCoords(pixelPos);
                view_.cursorOne.SetPosWorld(worldPos);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = view_.GetWindow().mapPixelToCoords(pixelPos);
                view_.cursorTwo.SetPosWorld(worldPos);
            }
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            if (event.mouseWheel.delta > 0)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos(event.mouseWheel.x, event.mouseWheel.y);
                // convert it to world coordinates
                sf::Vector2f worldPos = view_.GetWindow().mapPixelToCoords(pixelPos);
                view_.CentreOn(worldPos);
            }
            view_.Zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
        }

        if (isEnteringName_ or isEnteringFilter_)
        {
            HandleInputEventsWritingMode();
        }
        else
        {
            HandleInputEventsFreeMode();
        }
    }
}



void Controller::HandleInputEventsWritingMode()
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::BackSpace)
        {
            if (textBeingEntered_.length() > 0) textBeingEntered_.erase(textBeingEntered_.length()-1);
        }
        if (event.key.code == sf::Keyboard::Return or event.key.code == sf::Keyboard::Escape)
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                if (isEnteringName_) {
                    model_.AddName(cu1.GetPlan()->GetPlanID(), textBeingEntered_);
                }
                if (isEnteringFilter_) {
                    auto p = model_.EngageNameFilter(cu1.GetPlan(), textBeingEntered_);
                    CursorsGoHere(p);
                }
            }
            isEnteringName_ = false;
            isEnteringFilter_ = false;
            textBeingEntered_.clear();
        }
    }
    if (event.type == sf::Event::TextEntered and textEntryCooldownTimer_.getElapsedTime() > sf::milliseconds(200) )
    {
        sf::Uint32 ch = event.text.unicode;
        if (ch >= 0x61 and ch <= 0x7A) {  //convert lowercase to capitals
            ch -= 0x20;
        }
        if (    (ch >= 0x30 and ch <= 0x39) or  //numbers
                (ch >= 0x41 and ch <= 0x5A) or  //capitals
                (ch == 0x23)    )  //#
        {
            textBeingEntered_.append( sf::String{ch} );
        }
    }
    view_.SetTextEntering(isEnteringName_ or isEnteringFilter_, textBeingEntered_);
}



void Controller::HandleInputEventsFreeMode()
{
    //Keyboard Events
    if (event.type == sf::Event::KeyPressed)
    {
        //requires nothing
        if (event.key.code == sf::Keyboard::Comma)
        {
            model_.InnerTick();
        }
        if (event.key.code == sf::Keyboard::Period)
        {
            model_.OuterTick();
        }
        if (event.key.code == sf::Keyboard::Num1)
        {
            view_.SetHighlightingMode(1);
        }
        if (event.key.code == sf::Keyboard::Num2)
        {
            view_.SetHighlightingMode(2);
        }
        if (event.key.code == sf::Keyboard::Num3)
        {
            view_.SetHighlightingMode(3);
        }

        PlanShp plan1;
        if (cu1.GetState() != CursorState::ABSENT)
        {
            plan1 = cu1.GetPlan();
            EventsPlan(plan1);
        }
        if (cu1.GetState() == CursorState::LOCATED)
        {
            PlanPos pos1 = cu1.GetPlanPos();
            EventsLocated(pos1);
        }
        Shp<WiringPair> wp = RetrieveWiringPair(cu1, cu2);
        if (wp)
        {
            EventsBothWirable(wp);
        }
    }
}
            
            
            
void Controller::EventsPlan(PlanShp plan)
{
    //requires cu1 PLAN
    if (event.key.code == sf::Keyboard::X)
    {
        auto h = plan->GetHandle();
        if (h)
            h->SetExploded(false);
    }

    if (event.key.code == sf::Keyboard::LBracket)
    {
        view_.cursorOne.SetToPlan();
    }
    if (event.key.code == sf::Keyboard::RBracket)
    {
        view_.cursorTwo.SetToPlan();
    }

    if (event.key.code == sf::Keyboard::Q)
    {
        if (event.key.shift) model_.SavePlanAsNew( plan );
        else model_.SavePlan( plan );
    }
    if (event.key.code == sf::Keyboard::W)
    {
        auto p = model_.WipePlan(plan, event.key.shift and event.key.control);
        CursorsGoHere(p);
    }

    if (event.key.code == sf::Keyboard::Numpad1)
    {
        auto p = model_.LoadPlan(plan, PlanNav::PREV_ID);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad3)
    {
        auto p = model_.LoadPlan(plan, PlanNav::NEXT_ID);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad7)
    {
        auto p = model_.LoadPlan(plan, PlanNav::PREV_NAME);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad9)
    {
        auto p = model_.LoadPlan(plan, PlanNav::NEXT_NAME);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad8)
    {
        auto p = model_.LoadPlan(plan, PlanNav::PARENT);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad2)
    {
        auto p = model_.LoadPlan(plan, PlanNav::FIRST_CHILD);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad4)
    {
        auto p = model_.LoadPlan(plan, PlanNav::PREV_SIBLING);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Numpad6)
    {
        auto p = model_.LoadPlan(plan, PlanNav::NEXT_SIBLING);
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Subtract)
    {
        assert(not isEnteringName_);
        assert(not isEnteringFilter_);
        auto p = model_.EngageNameFilter(plan, "");
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Add)
    {
        assert(not isEnteringName_);
        assert(not isEnteringFilter_);
        isEnteringFilter_ = true;
        textBeingEntered_ = model_.GetNameFilter();
        textEntryCooldownTimer_.restart();
    }

    if (event.key.code == sf::Keyboard::R)
    {
        model_.RemoveName(plan->GetPlanID());
    }
    if (event.key.code == sf::Keyboard::E and model_.CanAddName(plan->GetPlanID()))
    {
        assert(not isEnteringName_);
        assert(not isEnteringFilter_);
        isEnteringName_ = true;
        textEntryCooldownTimer_.restart();
    }
}



void Controller::EventsLocated(PlanPos pos1)
{
    //requires cu1 LOCATED
    if (event.key.code == sf::Keyboard::S)
    {
        auto h = pos1.GetDeviceAsHandle();
        if (h)
            h->SetExploded(true);
    }

    if (event.key.code == sf::Keyboard::N)
    {
        if (event.key.shift == false)
        {
            model_.GetFactory()->AddNeuron(pos1);
        }
        else
        {
            model_.GetFactory()->RemoveDevice(pos1);
        }
    }
    if (event.key.code == sf::Keyboard::H)
    {
        model_.GetFactory()->AddHandle(pos1);
    }
    //if PlanPos sucessfully selects a device...
    DeviceShp dev1 = pos1.GetDevice();
    if (dev1)
    {
        if (event.key.code == sf::Keyboard::A)
        {
            view_.PostMessage("Tried to modify a device upwards");
            dev1->Handle(1);
        }
        if (event.key.code == sf::Keyboard::Z)
        {
            dev1->Handle(2);
        }
        if (event.key.code == sf::Keyboard::BackSlash)
        {
            dev1->Handle(3);
        }

        //if cu1 DEVICE + cu2 LOCATED
        if (cu2.GetState() == CursorState::LOCATED)
        {
            PlanPos pos2 = cu2.GetPlanPos();
            //EventsBothLocated(pos1, pos2);
            if (event.key.code == sf::Keyboard::M)
            {
                view_.PostMessage("Tried to move something");
                if (dev1->GetContainer() == pos2.GetPlan())
                    dev1->GetContainer()->SetPosition(dev1, pos2.GetSmartPos());
                //ChipPlanFunc::SetPosition(pos1, pos2); //allows moving of exploded plans
            }
        }
    }
}


void Controller::EventsBothLocated(PlanPos pos1, PlanPos pos2)
{
    //defunct
}



void Controller::EventsBothWirable(Shp<WiringPair> wp)
{
    //requires cu1 + cu2 in a VALID WIRING RELATIONSHIP
    if (event.key.code == sf::Keyboard::B)
    {
        if (event.key.shift == false)
        {
            model_.GetFactory()->AddWire(wp, 1);
        }
        else
        {
            model_.GetFactory()->RemoveWire(wp->plan, wp->from, wp->to);
        }
    }
    auto wire = wp->plan->GetWire(wp->from, wp->to);
    if (wire) 
    {
        if (event.key.code == sf::Keyboard::D) {
            wire->Handle(1);
        }
        if (event.key.code == sf::Keyboard::C) {
            wire->Handle(2);
        }
        if (event.key.code == sf::Keyboard::F) {
            wire->Handle(3);
        }
        if (event.key.code == sf::Keyboard::V) {
            wire->Handle(4);
        }
    }
}



void Controller::HandleInputState()
{
    if ( sf::Mouse::isButtonPressed(sf::Mouse::Middle) )
    {
        sf::Vector2i pixelPos{ sf::Mouse::getPosition(view_.GetWindow()) };
        sf::Vector2f worldPos{ view_.GetWindow().mapPixelToCoords( pixelPos ) };
        if (!isMouseCursorSet_) {
            isMouseCursorSet_ = true;
        }
        else {
            view_.Pan( (-worldPos + mouseCursorWorldPos_) /1.00f );
        }
        sf::Vector2i newPixelPos{ sf::Mouse::getPosition(view_.GetWindow()) };
        mouseCursorWorldPos_ = view_.GetWindow().mapPixelToCoords( newPixelPos );
    }
    else {
        isMouseCursorSet_ = false;
    }
}

