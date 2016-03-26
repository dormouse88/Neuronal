/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"
#include <cassert>

void TextEnterer::Append(sf::Uint32 ch)
{
    if (textEntryCooldownTimer_.getElapsedTime() > sf::milliseconds(200) )
    {
        //A few conversions...
        if (ch >= 0x61 and ch <= 0x7A)  //convert lowercase to capitals
            ch -= 0x20;
        if (ch == 0x20)                 //convert space to underscore
            ch = 0x5F;
        //Accepted characters...
        if (
                (ch >= 0x30 and ch <= 0x39) or  //numbers
                (ch >= 0x41 and ch <= 0x5A) or  //capitals
                (ch == 0x2B) or                 //+
                (ch == 0x5F)                    //_
            )
        {
            textBeingEntered_.append( sf::String{ch} );
        }
    }
}






Controller::Controller(Model & model_p, View & view_p)
    :model_(model_p)
    , view_(view_p)
    , quitYet_(false)
    , isMouseCursorSet_(false)
    , textEnterer_(nullptr)
    , cu1(view_.cursorOne)
    , cu2(view_.cursorTwo)
{}

void Controller::CursorsGoHere(PlanShp p)
{
//    if (p) {
//        view_.cursorOne.SetToPlan( p );
//        view_.cursorTwo.SetToPlan( p );
//        
//        auto b = p->GetWorldPaddedBoundBox();  //unimportant
//        view_.CentreOn( {b.left + b.width*0.5f, b.top + b.height} );
//        view_.Clamp();
//    }
}



bool Controller::HandleInput()
{
    HandleInputEvents();
    HandleInputState();
    cu1.Revalidate();
    cu2.Revalidate();
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
            //view_.Resize( sf::Vector2f{static_cast<float>(event.size.width), static_cast<float>(event.size.height)} );
            view_.SizingsRefresh();
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

        if (textEnterer_)
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
            textEnterer_->BackSpace();
        }
        if (event.key.code == sf::Keyboard::Return or event.key.code == sf::Keyboard::Escape)
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                textEnterer_->Dispatch();
            }
            textEnterer_ = nullptr;
        }
    }
    if (event.type == sf::Event::TextEntered)
    {
        if (textEnterer_)
            textEnterer_->Append(event.text.unicode);
    }
    if (textEnterer_)
        view_.SetTextEntering(true, textEnterer_->GetText());
    else
        view_.SetTextEntering(false);
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
        if (cu1.GetState() == CursorState::PORT)
        {
            plan1 = cu1.GetPlan();
            PortLocation pl = cu1.GetPort();
            if (event.key.code == sf::Keyboard::G)
            {
                assert(not textEnterer_);
                textEnterer_ = std::make_shared<TextEnterer>();
                auto bound = std::bind( &ChipPlan::RenamePortTag, plan1, pl, std::placeholders::_1 );
                textEnterer_->SetDispatchTarget( bound );
            }
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
        if (not event.key.shift and (model_.GetPlanGroupData()->GetNameByID(plan->GetPlanID()) != NULL_PLAN_NAME))  //ouch
            model_.SavePlan( plan, PlanNamingMode::TRANSFER );
        else;
            model_.SavePlan( plan, PlanNamingMode::AUTONAME );
    }
    if (event.key.code == sf::Keyboard::W)
    {
        auto p = model_.LoadPlan(plan, PlanNav::EMPTY, event.key.shift and event.key.control);
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
        assert(not textEnterer_);
        auto p = model_.EngageNameFilter(plan, "");
        CursorsGoHere(p);
    }
    if (event.key.code == sf::Keyboard::Add)
    {
        assert(not textEnterer_);
        textEnterer_ = std::make_shared<TextEnterer>();
        auto bound = std::bind( &Model::EngageNameFilter, &model_, cu1.GetPlan(), std::placeholders::_1 );
        textEnterer_->SetDispatchTarget( bound );
        textEnterer_->SetText( model_.GetNameFilter() );
    }

    if (event.key.code == sf::Keyboard::R)
    {
        assert(not textEnterer_);
        model_.RemoveName(plan->GetPlanID());
    }
    if (event.key.code == sf::Keyboard::E)// and model_.CanAddSomeRealName(plan->GetPlanID()))
    {
        assert(not textEnterer_);
        textEnterer_ = std::make_shared<TextEnterer>();
        auto bound = std::bind( &Model::SetRealName, &model_, cu1.GetPlan()->GetPlanID(), std::placeholders::_1 );
        textEnterer_->SetDispatchTarget( bound );
        textEnterer_->SetText( model_.GetCleanRealPlanName(cu1.GetPlan()->GetPlanID()) );
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
            pos1.GetPlan()->RemoveDevice(pos1);
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
            auto wire = wp->plan->GetWire(wp);
            if (wire)
                wp->plan->RemoveWire(wire);
        }
    }
    auto wire = wp->plan->GetWire(wp);
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

