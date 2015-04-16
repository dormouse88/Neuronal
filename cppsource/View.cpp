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
    mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.8f));
    barView.setViewport(sf::FloatRect(0.f, 0.8f, 1.f, 0.2f));
    window.setView(mainView);
}



void View::ImportDevice(std::shared_ptr<DeviceView> device)
{
    deviceViews.emplace_back(device);
}
void View::ExpelDevices()
{
    auto return_func = [] (std::shared_ptr<DeviceView> eachDevice) {return eachDevice->IsDead();};
    auto new_end = std::remove_if(std::begin(deviceViews), std::end(deviceViews), return_func );
    deviceViews.erase(new_end, std::end(deviceViews) );
}
void View::ImportWire(std::shared_ptr<WireView> wire)
{
    wireViews.emplace_back(wire);
}
void View::ExpelWires()
{
    auto remove_func = [] (std::shared_ptr<WireView> eachWire) {return eachWire->IsDead();};
    auto new_end = std::remove_if(std::begin(wireViews), std::end(wireViews), remove_func);
    wireViews.erase(new_end, std::end(wireViews) );
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
    for (auto &n: deviceViews) {
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
void View::Resize(sf::Vector2f newSize)
{
    mainView.setSize(newSize);
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

