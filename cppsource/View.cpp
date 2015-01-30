/* 
 * File:   View.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:56
 */

#include "View.hpp"

View::View(Model & model_p)
   :theModel(model_p),
    window(sf::VideoMode(800, 600), "Neuronal"),
    view(sf::Vector2f(250.f, 200.f), sf::Vector2f(800.f, 600.f) ),
    cursorOne(),
    cursorTwo(sf::Color::Cyan)
{
    model_p.AddListener(this);
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.8f));
    window.setView(view);
}

#include <iostream>
void View::OnNotify(bool added, Neuron * rp)
{
    std::cout << "View was notified of a Neuron." << std::endl;
    if (added) {
        std::unique_ptr<NeuronView> up{new NeuronView{*rp, vRes} };
        if (true) neuronViews.push_back( std::move(up) );
    }
}

void View::OnNotify(bool added, const Wire & cr)
{
    std::cout << "View was notified of a Wire." << std::endl;
    if (added) {
        std::unique_ptr<WireView> up{new WireView{cr, vRes} };
        if (true) wireViews.push_back( std::move(up) );
    }
}


void View::Draw()
{
    window.clear();
    for (auto &w: wireViews) {
        w->Draw(window);
    }
    for (auto &n: neuronViews) {
        n->Draw(window);
    }

    cursorTwo.Draw(window);
    cursorOne.Draw(window);
    window.display();
}

void View::Zoom(float zoomFactor)
{
    view.zoom( zoomFactor );
    window.setView(view);
}

void View::Pan(sf::Vector2f moveBy)
{
    view.move(moveBy);
    window.setView(view);
}

void View::SetCursorOnePos(sf::Vector2f pos)
{
    cursorOne.SetPos( mapGridToCoords( mapCoordsToGrid(pos) ) );
}

void View::SetCursorTwoPos(sf::Vector2f pos)
{
    cursorTwo.SetPos( mapGridToCoords( mapCoordsToGrid(pos) ) );
}

