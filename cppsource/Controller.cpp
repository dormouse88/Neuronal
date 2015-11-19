/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"

Controller::Controller(BlobFactory & factory_p, Serializer & serializer_p, Model & model_p, View & view_p)
    :theFactory(factory_p), theSerializer(serializer_p), theModel(model_p), theView(view_p), mouseCursorSet(false)
{}

void Controller::DebugInfo()
{
    //std::cout << "--CONT: " << "DEVI: " << deviceCons.size() << ", WIRE: " << wireCons.size() << std::endl;
}

bool Controller::ReplaceActivePlan(std::shared_ptr<ChipPlan> newPlan, std::shared_ptr<ChipPlan> activePlan)
{
    //This could all surely be better done...
    //Perhaps if the model acted as a referer for the base plan by the model implementing a Referer interface?
    if (newPlan)
    {
        auto handle = activePlan->GetReferer();
        //Basically, if (not the basePlan)...
        if (handle) {
            handle->SetPlan(newPlan);
            newPlan->RegisterReferer(handle);
        }
        //Pretty much the other case: if (it is the basePlan)...
        if (theModel.GetBasePlan() == activePlan) {
            theModel.SetBasePlan(newPlan);
        }
        //Finally update the view to point to the new plan...
        theView.SetActivePlan(newPlan);
        return true;
    }
    return false;
}
bool Controller::WipePlan(std::shared_ptr<ChipPlan> plan, bool forced)
{
    if (forced or not plan->IsModified())
    {
        auto emptyPlan = theFactory.MakePlan();
        if (ReplaceActivePlan(emptyPlan, plan)) return true;
    }
    return false;
}
bool Controller::LoadPlan(int num, std::shared_ptr<ChipPlan> activePlan)
{
    if (not activePlan->IsModified())
    {
        auto loadedPlan = theSerializer.LoadFile(theFactory, num);
        if (ReplaceActivePlan(loadedPlan, activePlan)) return true;
    }
    return false;
}


bool Controller::HandleInput()
{
    //std::shared_ptr<const PlanPos> pos1 = nullptr;
    PlanPos pos1;
    PlanPos pos2;
    std::shared_ptr<Wirable> wirable1 = nullptr;
    std::shared_ptr<Wirable> wirable2 = nullptr;
    std::shared_ptr<Device> device1 = nullptr;
    std::shared_ptr<Device> device2 = nullptr;
    std::shared_ptr<Wire> wire1 = nullptr;
        
    //Update handles...
    auto ctivePlan = theView.GetActivePlan().lock();
    if (ctivePlan) {
        pos1 = theView.cursorOne.GetPlanPos();
        if (not pos1.IsPlanOnly()) {
            device1 = pos1.GetPlan()->GetDevice( pos1.GetSmartPos() ); //device1 = activePlan->GetDevice( pos1.GetSmartPos() );
            wirable1 = device1;
        }
        else {
            wirable1 = pos1.GetPlan();  //activePlan;
        }
        pos2 = theView.cursorTwo.GetPlanPos();
        if (not pos2.IsPlanOnly()) {
            device2 = pos2.GetPlan()->GetDevice( pos2.GetSmartPos() ); //device2 = activePlan->GetDevice( pos2.GetSmartPos() );
            wirable2 = device2;
        }
        else {
            wirable2 = pos2.GetPlan();  //activePlan;
        }
        if (wirable1 and wirable2) {
            wire1 = pos1.GetPlan()->GetWire(*wirable1, *wirable2);   //activePlan->GetWire(*wirable1, *wirable2);
        }
    }
    theView.device1 = device1;
    theView.device2 = device2;
    theView.wire1 = wire1;
    
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
            //Check if clicked pos is inside plan boundary...
//            auto b = activePlan->GetWorldBound();
            //if (b.contains(newPos))

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
//                theView.cursorOne.SetPosWorld(worldPos, activePlan->GetGrid());
                theView.cursorOne.SetPosWorld(worldPos);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
//                theView.cursorTwo.SetPosWorld(worldPos, activePlan->GetGrid());
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
                theView.GetMainView().setCenter(worldPos.x, worldPos.y);
            }
            theView.Zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
        }
        
        //Keyboard Events
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::N)
            {
                if (event.key.shift == false) {
                    if (not pos1.IsPlanOnly()) theFactory.AddNeuron(pos1.GetPlan(), 0, pos1.GetSmartPos(), 1 );
                }
                else {
                    if (device1) pos1.GetPlan()->RemoveDevice(device1);
                }
            }
            if (event.key.code == sf::Keyboard::J)
            {
                if (not pos1.IsPlanOnly()) theFactory.AddJumper(pos1.GetPlan(), 0, pos1.GetSmartPos() );
            }
            if (event.key.code == sf::Keyboard::H)
            {
                if (not pos1.IsPlanOnly()) theFactory.AddHandle(pos1.GetPlan(), 0, pos1.GetSmartPos() );
            }
            if (event.key.code == sf::Keyboard::B)
            {
                if (event.key.shift == false) {
                    if (wirable1 and wirable2) theFactory.AddWire(pos1.GetPlan(), *wirable1, *wirable2, 1 );
                }
                else {
                    if (wire1) pos1.GetPlan()->RemoveWire( wire1 );
                }
            }
            if (event.key.code == sf::Keyboard::A) {
                if (device1) device1->Handle(1);
            }
            if (event.key.code == sf::Keyboard::Z) {
                if (device1) device1->Handle(2);
            }
            if (event.key.code == sf::Keyboard::D) {
                if (wire1) wire1->Handle(1);
            }
            if (event.key.code == sf::Keyboard::C) {
                if (wire1) wire1->Handle(2);
            }
            if (event.key.code == sf::Keyboard::F) {
                if (wire1) wire1->Handle(3);
            }
            if (event.key.code == sf::Keyboard::V) {
                if (wire1) wire1->Handle(4);
            }
            if (event.key.code == sf::Keyboard::S) {
                if (device1) {
                    std::shared_ptr<ChipHandle> handle = std::dynamic_pointer_cast<ChipHandle>(device1);
                    if (handle) {
                        theView.PushPlan( handle->GetPlan() );
                    }
                }
            }
            if (event.key.code == sf::Keyboard::X) {
                theView.PopPlan();
            }
            if (event.key.code == sf::Keyboard::M) {
                if (device1 and pos2.IsLocated()) pos2.GetPlan()->SetPosition( *device1, pos2.GetSmartPos() );
            }
            if (event.key.code == sf::Keyboard::Q)
            {
                theSerializer.SaveFile(pos1.GetPlan());
            }
            if (event.key.code == sf::Keyboard::W)
            {
                WipePlan(pos1.GetPlan(), event.key.shift and event.key.control);
            }
            if (event.key.code == sf::Keyboard::Dash)
            {
                if ( LoadPlan(pos1.GetPlan()->GetPlanID() - 1, pos1.GetPlan()) ) theView.xmlPlan -= 1;
            }
            if (event.key.code == sf::Keyboard::Equal)
            {
                if ( LoadPlan(pos1.GetPlan()->GetPlanID() + 1, pos1.GetPlan()) ) theView.xmlPlan += 1;
            }
            if (event.key.code == sf::Keyboard::LBracket)
            {
                theView.cursorOne.Nullify();
            }
            if (event.key.code == sf::Keyboard::RBracket)
            {
                theView.cursorTwo.Nullify();
            }
            if (event.key.code == sf::Keyboard::E)
            {
                theModel.DebugInfo();
                theView.DebugInfo();
                DebugInfo();
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
    
    return quitYet;
}

