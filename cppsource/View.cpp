/* 
 * File:   View.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:56
 */

#include "View.hpp"
#include <iostream>

const sf::FloatRect MAIN_VIEWPORT {0.f, 0.f, 1.f, 0.8f};
const sf::FloatRect BAR_VIEWPORT {0.f, 0.8f, 1.f, 0.2f};

View::View(Model & model_p)
   :theModel(model_p),
    window(sf::VideoMode(800, 600), "Neuronal"),
    mainView(sf::Vector2f(250.f, 200.f), sf::Vector2f(800.f, 600.f) ),
    overlayBox(sf::Vector2f{1000.f, 1000.f} ),
    cursorOne(),
    cursorTwo(sf::Color::Cyan),
    highlightingMode(1)
{
    window.setVerticalSyncEnabled(true);
    mainView.setViewport(MAIN_VIEWPORT);
    mainOverlay.setViewport(MAIN_VIEWPORT);
    barOverlay.setViewport(BAR_VIEWPORT);
    window.setView(mainView);
    
    activePlan.push( model_p.GetBasePlan() );
}

void View::DebugInfo()
{
    //std::cout << "-VIEW: " << "DEVI: " << deviceViews.size() << ", WIRE: " << wireViews.size() << std::endl;
}


void View::Draw()
{
    window.clear();
    
    window.setView(barOverlay);
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
    auto ap = GetActivePlan().lock();
    if (ap)
    {
        ap->Draw(window);

        if (highlightingMode != 1)
        {
            window.setView(mainOverlay);
            overlayBox.setFillColor( sf::Color{0,0,0,210} );
            window.draw(overlayBox);
            window.setView(mainView);
        }
        if (highlightingMode == 2)
        {
            auto d1 = device1.lock();
            if (d1) d1->Draw(window);
            auto d2 = device2.lock();
            if (d2) d2->Draw(window);
            auto w1 = wire1.lock();
            if (w1) w1->Draw(window);
        }
        if (highlightingMode == 3)
        {
            auto d1 = device1.lock();
            if (d1) {
                d1->Draw(window);
                auto vec = ap->GetWires(d1, true, true);
                for (auto x: vec) {
                    x->Draw(window);
                }
            }
        }

    
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
void View::Resize(sf::Vector2f newSize)
{
    mainView.setSize(newSize);
    window.setView(mainView);
}


