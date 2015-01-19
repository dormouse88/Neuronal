/* 
 * File:   App.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 15:04
 */

#include "App.hpp"
#include <iostream> //del

App::App()
    :model(),
    view(model),
    controller(view, model)
{
}

void App::Run()
{
    MakeSomeStuff();
    bool quitYet = false;
    while (quitYet == false)
    {
        quitYet = controller.HandleInput();
        view.Draw();
    }
}

void App::MakeSomeStuff()
{
    model.AddNeuron(sf::Vector2i(1,1) );
    model.AddNeuron(sf::Vector2i(2,2) );
    model.AddNeuron(sf::Vector2i(3,4) );
    model.AddNeuron(sf::Vector2i(7,8) );
    model.SetThreshold(sf::Vector2i(1,1), 0);
    model.AddWire(sf::Vector2i(2,2), sf::Vector2i(3,4));
    model.AddWire(sf::Vector2i(1,1), sf::Vector2i(2,2));
    model.AddWire(sf::Vector2i(2,2), sf::Vector2i(7,8));
}

