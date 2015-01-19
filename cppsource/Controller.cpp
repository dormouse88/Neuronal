/* 
 * File:   Controller.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 12:42
 */

#include "Controller.hpp"

Controller::Controller(View & view_p, Model & model_p)
    :theView(view_p), theModel(model_p)
{}

bool Controller::HandleInput()
{
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
            if (event.key.code == sf::Keyboard::A) {
                theModel.ModifyThreshold( mapCoordsToGrid(theView.GetCursorOnePos() ), +1 );
            }
            if (event.key.code == sf::Keyboard::Z) {
                theModel.ModifyThreshold( mapCoordsToGrid(theView.GetCursorOnePos() ), -1 );
            }
            if (event.key.code == sf::Keyboard::M) {
                theModel.SetPosition( mapCoordsToGrid(theView.GetCursorOnePos()), mapCoordsToGrid(theView.GetCursorTwoPos()) );
            }
            if (event.key.code == sf::Keyboard::B)
            {
                theModel.AddWire( mapCoordsToGrid(theView.GetCursorOnePos()), mapCoordsToGrid(theView.GetCursorTwoPos()) );
            }
            if (event.key.code == sf::Keyboard::Space)
            {
                theModel.Logic();
            }
        }
        
    }
    return quitYet;
}

