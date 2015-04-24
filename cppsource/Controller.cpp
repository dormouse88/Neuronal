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
    //cursor selected objects...
    sf::Vector2i cursor1pos = mapCoordsToGrid(theView.GetCursorOnePos());
    sf::Vector2i cursor2pos = mapCoordsToGrid(theView.GetCursorTwoPos());
    std::shared_ptr<Device> device1 = theModel.GetDevice( cursor1pos );
    std::shared_ptr<Device> device2 = theModel.GetDevice( cursor2pos );
    std::shared_ptr<DeviceCon> dc1 = nullptr;
    if (device1) dc1 = GetDevice(device1);
    std::shared_ptr<DeviceCon> dc2 = nullptr;
    if (device2) dc2 = GetDevice(device2);
    std::shared_ptr<Wire> wire1 = nullptr;
    if (device1 and device2) wire1 = theModel.GetWire(*device1, *device2);
    std::shared_ptr<WireCon> wc1 = nullptr;
    if (wire1) wc1 = GetWire(wire1);
    
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
                theView.SetCursorOnePos(worldPos);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
                theView.SetCursorTwoPos(worldPos);
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
                    theFactory->AddNeuron( cursor1pos );
                }
                else {
                    theFactory->RemoveDevice( device1 );
                }
            }
            if (event.key.code == sf::Keyboard::H)
            {
                theFactory->AddSocket( cursor1pos );
            }
            if (event.key.code == sf::Keyboard::B)
            {
                if (event.key.shift == false) {
                    if (device1 != nullptr and device2 != nullptr) {
                        theFactory->AddWire( *device1, *device2 );
                    }
                }
                else {
                    if (wire1 != nullptr) {
                        theFactory->RemoveWire( wire1 );
                    }
                }
            }
            if (event.key.code == sf::Keyboard::A) {
                if (dc1 != nullptr) {
                    dc1->Handle(1);
                }
            }
            if (event.key.code == sf::Keyboard::Z) {
                if (dc1 != nullptr) {
                    dc1->Handle(2);
                }
            }
            if (event.key.code == sf::Keyboard::S) {
                if (wc1 != nullptr) {
                    wc1->Handle(1);
                }
            }
            if (event.key.code == sf::Keyboard::X) {
                if (wc1 != nullptr) {
                    wc1->Handle(2);
                }
            }
            if (event.key.code == sf::Keyboard::M) {
                if (device1 != nullptr) {
                    theModel.SetPosition( *device1, cursor2pos );
                }
            }
            if (event.key.code == sf::Keyboard::Q)
            {
                theModel.SaveXML();
            }
            if (event.key.code == sf::Keyboard::W)
            {
                theModel.LoadXML();
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


std::shared_ptr<DeviceCon> Controller::GetDevice(std::shared_ptr<Device> d)
{
    for (auto &c: deviceCons) {
        if (*d == c->GetDevice()) return c;
    }
    return nullptr;
}
std::shared_ptr<WireCon> Controller::GetWire(std::shared_ptr<Wire> w)
{
    for (auto &c: wireCons) {
        if (*w == c->GetWire()) return c;
    }
    return nullptr;
}


void Controller::ImportDevice(std::shared_ptr<DeviceCon> device)
{
    deviceCons.emplace_back(device);
}
void Controller::ExpelDevices()
{
    auto return_func = [] (std::shared_ptr<DeviceCon> eachDevice) {return eachDevice->IsDead();};
    auto new_end = std::remove_if(std::begin(deviceCons), std::end(deviceCons), return_func );
    deviceCons.erase(new_end, std::end(deviceCons) );
}
void Controller::ImportWire(std::shared_ptr<WireCon> wire)
{
    wireCons.emplace_back(wire);
}
void Controller::ExpelWires()
{
    auto remove_func = [] (std::shared_ptr<WireCon> eachWire) {return eachWire->IsDead();};
    auto new_end = std::remove_if(std::begin(wireCons), std::end(wireCons), remove_func);
    wireCons.erase(new_end, std::end(wireCons) );
}
