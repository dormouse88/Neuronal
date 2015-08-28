/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"

Controller::Controller(BlobFactory & factory_p, Model & model_p, View & view_p)
    :theFactory(factory_p), theModel(model_p), theView(view_p), mouseCursorSet(false)
{}

void Controller::DebugInfo()
{
    //std::cout << "--CONT: " << "DEVI: " << deviceCons.size() << ", WIRE: " << wireCons.size() << std::endl;
}

bool Controller::HandleInput()
{
    std::shared_ptr<sf::Vector2i> pos1 = nullptr;
    std::shared_ptr<sf::Vector2i> pos2 = nullptr;
    std::shared_ptr<Device> device1 = nullptr;
    std::shared_ptr<Device> device2 = nullptr;
    std::shared_ptr<Wire> wire1 = nullptr;
    //Update handles...
    auto activePlan = theView.GetActivePlan().lock();
    if (activePlan) {
        if (theView.cursorOne.active) {
            pos1 = std::make_shared<sf::Vector2i> ( theView.cursorOne.GetGridPos() );
        }
        if (theView.cursorTwo.active) {
            pos2 = std::make_shared<sf::Vector2i> ( theView.cursorTwo.GetGridPos() );
        }
        if (pos1) {
            device1 = activePlan->GetDevice( *pos1 );
        }
        if (pos2) {
            device2 = activePlan->GetDevice( *pos2 );
        }
        if (device1 and device2) {
            wire1 = activePlan->GetWire(*device1, *device2);
        }
    }
    
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
                theView.cursorOne.SetWorldPos(worldPos);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
                theView.cursorTwo.SetWorldPos(worldPos);
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
                    if (pos1) theFactory.AddNeuron(activePlan, 0, *pos1, 1 );
                }
                else {
                    if (device1) theFactory.RemoveDevice(activePlan, device1);
                }
            }
            if (event.key.code == sf::Keyboard::J)
            {
                if (pos1) theFactory.AddSocket(activePlan, 0, *pos1 );
            }
            if (event.key.code == sf::Keyboard::H)
            {
                if (pos1) theFactory.AddHandle(activePlan, 0, *pos1 );
            }
            if (event.key.code == sf::Keyboard::B)
            {
                if (event.key.shift == false) {
                    if (device1 and device2) theFactory.AddWire(activePlan, *device1, 0, *device2, 0, 1 ); //hack (fromSlot + toSlot zeroed)
                }
                else {
                    if (wire1) theFactory.RemoveWire(activePlan, wire1 );
                }
            }
            if (event.key.code == sf::Keyboard::A) {
                if (device1) device1->Handle(1);
            }
            if (event.key.code == sf::Keyboard::Z) {
                if (device1) device1->Handle(2);
            }
            if (event.key.code == sf::Keyboard::S) {
                if (wire1) wire1->Handle(1);
            }
            if (event.key.code == sf::Keyboard::X) {
                if (wire1) wire1->Handle(2);
            }
            if (event.key.code == sf::Keyboard::M) {
                if (device1 and pos2) activePlan->SetPosition( *device1, *pos2 );
            }
            if (event.key.code == sf::Keyboard::Q)
            {
                //theSerializer->Save(activePlan);
            }
            if (event.key.code == sf::Keyboard::W)
            {
                //theSerializer->Load(activePlan);
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

//if (plan)
//{
//    saveXML
//    if (pos1)
//    {
//        device-add
//        if (device1)
//        {
//            device-del
//            device-mod
//            loadXML;
//            if (pos2)
//            {
//                move
//                if (device2)
//                {
//                    add-wire
//                    if (wire1)
//                    {
//                        wire-del
//                        wire-mod
//                    }//wire1
//                }//device2
//            }//pos2
//        }//device1
//    }//pos1
//}//plan
//logic
//debug
