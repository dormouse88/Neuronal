/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"
#include <cassert>






//Controller::Controller(Model & model_p, View & view_p)
//    :model_(model_p)
//    , view_(view_p)
//    , quitYet_(false)
//    , isMouseCursorSet_(false)
//{}
//





//void Controller::HandleInputEventsWritingMode()
//{
//    if (event.type == sf::Event::KeyPressed)
//    {
//        if (event.key.code == sf::Keyboard::BackSpace)
//        {
//            textEnterer_->BackSpace();
//        }
//        if (event.key.code == sf::Keyboard::Return or event.key.code == sf::Keyboard::Escape)
//        {
//            if (event.key.code == sf::Keyboard::Return)
//            {
//                textEnterer_->Dispatch();
//            }
//            textEnterer_ = nullptr;
//        }
//    }
//    if (event.type == sf::Event::TextEntered)
//    {
//        if (textEnterer_)
//            textEnterer_->Append(event.text.unicode);
//    }
//    if (textEnterer_)
//        view_.SetTextEntering(true, textEnterer_->GetText());
//    else
//        view_.SetTextEntering(false);
//}


//void Controller::HandleInputEventsFreeMode()
//{
//    //Keyboard Events
//    if (event.type == sf::Event::KeyPressed)
//    {
//        //requires nothing...
//        if (event.key.code == sf::Keyboard::Num0)
//        {
//            view_.ToggleFieldMode();
//        }
//        
//        //requires arena...
//        if (event.key.code == sf::Keyboard::Period)
//        {
//            //model_.OuterTick();
//            arena->TimeAdvance();
//        }
//        
//        //requires brain...
//        if (event.key.code == sf::Keyboard::Comma)
//        {
//            //model_.InnerTick();
//            if (not brain->IsAnyOutputOn())
//                brain->TickOnce();
//        }
//        if (event.key.code == sf::Keyboard::Num1)
//        {
//            view_.SetHighlightingMode(1);
//        }
//        if (event.key.code == sf::Keyboard::Num2)
//        {
//            view_.SetHighlightingMode(2);
//        }
//        if (event.key.code == sf::Keyboard::Num3)
//        {
//            view_.SetHighlightingMode(3);
//        }
//
//        //Brain sections...
//        PlanShp plan1;
//        if (cu1.GetState() != CursorState::ABSENT)
//        {
//            plan1 = cu1.GetPlan();
//            EventsPlan(plan1);
//        }
//        if (cu1.GetState() == CursorState::PORT)
//        {
//            plan1 = cu1.GetPlan();
//            PortLocation pl = cu1.GetPort();
//            if (event.key.code == sf::Keyboard::G)
//            {
//                assert(not textEnterer_);
//                textEnterer_ = std::make_shared<TextEnterer>();
//                auto bound = std::bind( &ChipPlan::RenamePortTag, plan1, pl, std::placeholders::_1 );
//                textEnterer_->SetDispatchTarget( bound );
//            }
//        }
//        if (cu1.GetState() == CursorState::LOCATED)
//        {
//            PlanPos pos1 = cu1.GetPlanPos();
//            EventsLocated(pos1);
//        }
//        Shp<WiringPair> wp = RetrieveWiringPair(cu1, cu2);
//        if (wp)
//        {
//            EventsBothWirable(wp);
//        }
//    }
//}





























//void Controller::EventsPlan(PlanShp plan)
//{
//    //requires cu1 PLAN
//    if (event.key.code == sf::Keyboard::X)
//    {
//        auto h = plan->GetHandle();
//        if (h)
//            h->SetExploded(false);
//    }
//
//    if (event.key.code == sf::Keyboard::LBracket)
//    {
//        view_.cursorOne.SetToPlan();
//    }
//    if (event.key.code == sf::Keyboard::RBracket)
//    {
//        view_.cursorTwo.SetToPlan();
//    }
//
//    if (event.key.code == sf::Keyboard::Q)
//    {
//        if (not event.key.shift and (model_.GetPlanGroupData()->GetNameByID(plan->GetPlanID()) != NULL_PLAN_NAME))  //ouch
//            model_.SavePlan( plan, PlanNamingMode::TRANSFER );
//        else;
//            model_.SavePlan( plan, PlanNamingMode::AUTONAME );
//    }
//    if (event.key.code == sf::Keyboard::W)
//    {
//        model_.LoadPlan(plan, PlanNav::EMPTY, event.key.shift and event.key.control);
//    }
//
//    if (event.key.code == sf::Keyboard::Numpad1)
//    {
//        model_.LoadPlan(plan, PlanNav::PREV_ID);
//    }
//    if (event.key.code == sf::Keyboard::Numpad3)
//    {
//        model_.LoadPlan(plan, PlanNav::NEXT_ID);
//    }
//    if (event.key.code == sf::Keyboard::Numpad7)
//    {
//        model_.LoadPlan(plan, PlanNav::PREV_NAME);
//    }
//    if (event.key.code == sf::Keyboard::Numpad9)
//    {
//        model_.LoadPlan(plan, PlanNav::NEXT_NAME);
//    }
//    if (event.key.code == sf::Keyboard::Numpad8)
//    {
//        model_.LoadPlan(plan, PlanNav::PARENT);
//    }
//    if (event.key.code == sf::Keyboard::Numpad2)
//    {
//        model_.LoadPlan(plan, PlanNav::FIRST_CHILD);
//    }
//    if (event.key.code == sf::Keyboard::Numpad4)
//    {
//        model_.LoadPlan(plan, PlanNav::PREV_SIBLING);
//    }
//    if (event.key.code == sf::Keyboard::Numpad6)
//    {
//        model_.LoadPlan(plan, PlanNav::NEXT_SIBLING);
//    }
//    if (event.key.code == sf::Keyboard::Subtract)
//    {
//        assert(not textEnterer_);
//        model_.EngageNameFilter(plan, "");
//    }
//    if (event.key.code == sf::Keyboard::Add)
//    {
//        assert(not textEnterer_);
//        textEnterer_ = std::make_shared<TextEnterer>();
//        auto bound = std::bind( &Model::EngageNameFilter, &model_, cu1.GetPlan(), std::placeholders::_1 );
//        textEnterer_->SetDispatchTarget( bound );
//        textEnterer_->SetText( model_.GetNameFilter() );
//    }
//
//    if (event.key.code == sf::Keyboard::R)
//    {
//        assert(not textEnterer_);
//        model_.RemoveName(plan->GetPlanID());
//    }
//    if (event.key.code == sf::Keyboard::E)// and model_.CanAddSomeRealName(plan->GetPlanID()))
//    {
//        assert(not textEnterer_);
//        textEnterer_ = std::make_shared<TextEnterer>();
//        auto bound = std::bind( &Model::SetRealName, &model_, cu1.GetPlan()->GetPlanID(), std::placeholders::_1 );
//        textEnterer_->SetDispatchTarget( bound );
//        textEnterer_->SetText( model_.GetCleanRealPlanName(cu1.GetPlan()->GetPlanID()) );
//    }
//}
//
//
//
//void Controller::EventsLocated(PlanPos pos1)
//{
//    //requires cu1 LOCATED
//    if (event.key.code == sf::Keyboard::S)
//    {
//        auto h = pos1.GetDeviceAsHandle();
//        if (h)
//            h->SetExploded(true);
//    }
//
//    if (event.key.code == sf::Keyboard::N)
//    {
//        if (event.key.shift == false)
//        {
//            model_.GetFactory()->AddNeuron(pos1);
//        }
//        else
//        {
//            pos1.GetPlan()->RemoveDevice(pos1);
//        }
//    }
//    if (event.key.code == sf::Keyboard::H)
//    {
//        model_.GetFactory()->AddHandle(pos1);
//    }
//    //if PlanPos sucessfully selects a device...
//    DeviceShp dev1 = pos1.GetDevice();
//    if (dev1)
//    {
//        if (event.key.code == sf::Keyboard::A)
//        {
//            view_.PostMessage("Tried to modify a device upwards");
//            dev1->Handle(1);
//        }
//        if (event.key.code == sf::Keyboard::Z)
//        {
//            dev1->Handle(2);
//        }
//        if (event.key.code == sf::Keyboard::BackSlash)
//        {
//            dev1->Handle(3);
//        }
//
//        //if cu1 DEVICE + cu2 LOCATED
//        if (cu2.GetState() == CursorState::LOCATED)
//        {
//            PlanPos pos2 = cu2.GetPlanPos();
//            //EventsBothLocated(pos1, pos2);
//            if (event.key.code == sf::Keyboard::M)
//            {
//                view_.PostMessage("Tried to move something");
//                if (dev1->GetContainer() == pos2.GetPlan())
//                    dev1->GetContainer()->SetPosition(dev1, pos2.GetSmartPos());
//                //ChipPlanFunc::SetPosition(pos1, pos2); //allows moving of exploded plans
//            }
//        }
//    }
//}
//
//
//void Controller::EventsBothLocated(PlanPos pos1, PlanPos pos2)
//{
//    //defunct
//}
//
//
//
//void Controller::EventsBothWirable(Shp<WiringPair> wp)
//{
//    //requires cu1 + cu2 in a VALID WIRING RELATIONSHIP
//    if (event.key.code == sf::Keyboard::B)
//    {
//        if (event.key.shift == false)
//        {
//            model_.GetFactory()->AddWire(wp, 1);
//        }
//        else
//        {
//            auto wire = wp->plan->GetWire(wp);
//            if (wire)
//                wp->plan->RemoveWire(wire);
//        }
//    }
//    auto wire = wp->plan->GetWire(wp);
//    if (wire) 
//    {
//        if (event.key.code == sf::Keyboard::D) {
//            wire->Handle(1);
//        }
//        if (event.key.code == sf::Keyboard::C) {
//            wire->Handle(2);
//        }
//        if (event.key.code == sf::Keyboard::F) {
//            wire->Handle(3);
//        }
//        if (event.key.code == sf::Keyboard::V) {
//            wire->Handle(4);
//        }
//    }
//}
//
//
//
//void Controller::HandleInputState()
//{
//    if ( sf::Mouse::isButtonPressed(sf::Mouse::Middle) )
//    {
//        sf::Vector2i pixelPos{ sf::Mouse::getPosition(view_.GetWindow()) };
//        sf::Vector2f worldPos{ view_.GetWindow().mapPixelToCoords( pixelPos ) };
//        if (!isMouseCursorSet_) {
//            isMouseCursorSet_ = true;
//        }
//        else {
//            view_.Pan( (-worldPos + mouseCursorWorldPos_) /1.00f );
//        }
//        sf::Vector2i newPixelPos{ sf::Mouse::getPosition(view_.GetWindow()) };
//        mouseCursorWorldPos_ = view_.GetWindow().mapPixelToCoords( newPixelPos );
//    }
//    else {
//        isMouseCursorSet_ = false;
//    }
//}
//
