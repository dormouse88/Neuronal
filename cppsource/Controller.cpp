/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"

Controller::Controller(View & view_p, Model & model_p)
    :theView(view_p), theModel(model_p), mouseCursorSet(false)
{}

bool Controller::HandleInput()
{
    //cursor selected objects...
    sf::Vector2i cursor1pos = mapCoordsToGrid(theView.GetCursorOnePos());
    sf::Vector2i cursor2pos = mapCoordsToGrid(theView.GetCursorTwoPos());
    Neuron * neuron1 = theModel.GetNeuron( cursor1pos );
    Neuron * neuron2 = theModel.GetNeuron( cursor2pos );
    Wire * wire1 = theModel.GetWire(cursor1pos, cursor2pos);
    
    bool quitYet = false;
    sf::Event event;
    while (theView.GetWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            quitYet = true;
        
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
                theView.GetView().setCenter(worldPos.x, worldPos.y);
            }
            theView.Zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
        }
        
        //Keyboard Events
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::N)
            {
                theModel.AddNeuron( mapCoordsToGrid( theView.GetCursorOnePos() ) );
            }
            if (event.key.code == sf::Keyboard::B)
            {
                theModel.AddWire( mapCoordsToGrid(theView.GetCursorOnePos()), mapCoordsToGrid(theView.GetCursorTwoPos()) );
            }
            if (event.key.code == sf::Keyboard::A) {
                if (neuron1 != nullptr) {
                    neuron1->SetThreshold( neuron1->GetThreshold() + 1);
                }
            }
            if (event.key.code == sf::Keyboard::Z) {
                if (neuron1 != nullptr) {
                    neuron1->SetThreshold( neuron1->GetThreshold() - 1);
                }
            }
            if (event.key.code == sf::Keyboard::S) {
                if (wire1 != nullptr) {
                    wire1->SetWeight( wire1->GetWeight() + 1);
                }
            }
            if (event.key.code == sf::Keyboard::X) {
                if (wire1 != nullptr) {
                    wire1->SetWeight( wire1->GetWeight() - 1);
                }
            }
            if (event.key.code == sf::Keyboard::M) {
                if (neuron1 != nullptr) {
                    theModel.SetPosition( *neuron1, cursor2pos );
                }
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

