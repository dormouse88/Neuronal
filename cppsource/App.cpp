/* 
 * File:   App.cpp
 * Author: Dormouse
 * 
 * Created on 13 January 2015, 15:04
 */

#include "App.hpp"
#include <iostream>

App::App()
    :window(sf::VideoMode(800, 600), "SFML works!"),
        view(sf::Vector2f(250.f, 300.f), sf::Vector2f(300.f, 200.f) )
{
    view.setViewport(sf::FloatRect(0.05, 0.05f, 0.9f, 0.7f));
    window.setView(view);
    
    nBox.Add(10,150);
    nBox.Add(150,50);
    nBox.Add(270,300);
    nBox.Add(420,190);
    nBox.n_vector[0]->SetThreshold(0);
    std::unique_ptr<Wire> w1(new Wire(*nBox.n_vector[1], *nBox.n_vector[2]));
    wires.emplace_back( std::move(w1) );
    wires.emplace_back( std::unique_ptr<Wire>{new Wire(*nBox.n_vector[0], *nBox.n_vector[1]) } );
    wires.emplace_back( std::unique_ptr<Wire>{new Wire(*nBox.n_vector[1], *nBox.n_vector[3]) } );
}

void App::Run()
{
    while (window.isOpen())
    {
        HandleInput();
        Draw();
    }
}

void App::Logic()
{
    for (auto & n : nBox.n_vector) {
        n->StepPartOne();
    }
    for (auto & n : nBox.n_vector) {
        n->StepPartTwo();
    }
}

void App::HandleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                // convert it to world coordinates
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                //quantize to 10
                worldPos.x = round(worldPos.x / 75.f) * 75.f;
                worldPos.y = round(worldPos.y / 75.f) * 75.f;
                
                nBox.Add(worldPos.x, worldPos.y);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                Logic();
            }
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            if (event.mouseWheel.delta > 0)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos(event.mouseWheel.x, event.mouseWheel.y);
                // convert it to world coordinates
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                view.setCenter(worldPos.x, worldPos.y);
            }
            view.zoom( 1.f + (-0.4f * event.mouseWheel.delta) );
            window.setView(view);
        }
    }
}

void App::Draw()
{
    window.clear();
    for (auto &w: wires) {
        w->Draw(window);
    }
    for (auto &n: nBox.n_vector) {
        n->Draw(window);
    }

    window.display();
}

