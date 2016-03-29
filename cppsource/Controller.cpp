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
//
//void PaneBrain::HandleInputState()
//{
//    mousePos = sf::Mouse::getPosition;
//    if (mouse moved)
//    {
//        if (mid is pressed)
//            pan( (world)mousePos - (world)prevMousePos_ );
//    }
//    prevMousePos_ = mousePos;
//}

        