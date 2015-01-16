/* 
 * File:   View.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:56
 */

#include "View.hpp"

View::View(Model & model_p)
   :theModel(model_p),
    window(sf::VideoMode(800, 600), "SFML works!"),
    view(sf::Vector2f(250.f, 300.f), sf::Vector2f(300.f, 200.f) )
{
    model_p.AddListener(this);
    view.setViewport(sf::FloatRect(0.05, 0.05f, 0.9f, 0.7f));
    window.setView(view);
}

#include <iostream>
void View::OnNotify(bool added, Neuron * rp)
{
    
    std::cout << "I was notified." << std::endl;
    if (added) {
        std::unique_ptr<NeuronView> up{new NeuronView{*rp, vRes} };
        if (true) neuronViews.push_back( std::move(up) );
        
        std::cout << "..." << std::endl;
        std::cout << "...and I survived" << std::endl;
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

    window.display();
}





