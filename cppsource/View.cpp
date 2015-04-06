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
    mainView(sf::Vector2f(250.f, 200.f), sf::Vector2f(800.f, 600.f) ),
    cursorOne(),
    cursorTwo(sf::Color::Cyan)
{
    model_p.AddListener(this);
    mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.8f));
    barView.setViewport(sf::FloatRect(0.f, 0.8f, 1.f, 0.2f));
    window.setView(mainView);
}

#include <iostream>
void View::OnNotify(bool added, Neuron * rp)
{
    if (added) {
        std::cout << "View was notified of: Neuron Added." << std::endl;
        std::unique_ptr<NeuronView> up{new NeuronView{*rp, vRes} };
        neuronViews.push_back( std::move(up) );
    }
    else {
        std::cout << "View was notified of: Neuron Removed." << std::endl;
        auto killMe = [&] (std::unique_ptr<NeuronView> & nv) {return nv->AmIYourDaddy(*rp);} ;
        neuronViews.erase( std::remove_if(std::begin(neuronViews), std::end(neuronViews), killMe), std::end(neuronViews) );
    }
}

void View::OnNotify(bool added, const Wire & cr)
{
    if (added) {
        std::cout << "View was notified of: Wire   Added." << std::endl;
        std::unique_ptr<WireView> up{new WireView{cr, vRes} };
        wireViews.push_back( std::move(up) );
    }
    else {
        std::cout << "View was notified of: Wire   Removed." << std::endl;
        auto killMe = [&] (std::unique_ptr<WireView> & wv) {return wv->AmIYourDaddy(cr);} ;
        wireViews.erase( std::remove_if(std::begin(wireViews), std::end(wireViews), killMe), std::end(wireViews) );
    }
}


void View::Draw()
{
    window.clear();
    window.setView(barView);
    
//    sf::RectangleShape bar;
//    bar.setSize( sf::Vector2f{ barView.getViewport().width, barView.getViewport().height } );
//    bar.setPosition( sf::Vector2f{ barView.getViewport().left, barView.getViewport().top } );
//    bar.setFillColor(sf::Color::Blue);
    
    sf::VertexArray lines(sf::Quads, 4);
    lines[0].position = sf::Vector2f(0, 0);
    lines[1].position = sf::Vector2f(1000, 0);
    lines[2].position = sf::Vector2f(1000, 1000);
    lines[3].position = sf::Vector2f(0, 1000);
    lines[0].color = sf::Color::Red;
    lines[2].color = sf::Color::Yellow;
    window.draw(lines);

    
    
    window.setView(mainView);
    for (auto &n: neuronViews) {
        n->Draw(window);
    }
    for (auto &w: wireViews) {
        w->Draw(window);
    }
    cursorTwo.Draw(window);
    cursorOne.Draw(window);

    window.display();
}

void View::Zoom(float zoomFactor)
{
    mainView.zoom( zoomFactor );
    window.setView(mainView);
}

void View::Pan(sf::Vector2f moveBy)
{
    mainView.move(moveBy);
    window.setView(mainView);
}

void View::SetCursorOnePos(sf::Vector2f pos)
{
    cursorOne.SetPos( mapGridToCoords( mapCoordsToGrid(pos) ) );
}

void View::SetCursorTwoPos(sf::Vector2f pos)
{
    cursorTwo.SetPos( mapGridToCoords( mapCoordsToGrid(pos) ) );
}

