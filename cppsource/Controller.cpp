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
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                
                // convert it to world coordinates
                sf::Vector2f worldPos = theView.GetWindow().mapPixelToCoords(pixelPos);
               
                theModel.AddNeuron( mapCoordsToGrid(worldPos) );
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                theModel.Logic();
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
            theView.GetView().zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
            theView.GetWindow().setView(theView.GetView());
        }
    }
    return quitYet;
}

