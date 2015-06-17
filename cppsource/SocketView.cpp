/* 
 * File:   SocketView.cpp
 * Author: Dormouse
 * 
 * Created on 16 April 2015, 15:45
 */

#include "SocketView.hpp"

const float SOCKET_SIZE {20.f};
const sf::Vector2f SOCKET_OFFSET { (GRID_SIZE/2.f) - sf::Vector2f(SOCKET_SIZE, SOCKET_SIZE) };

SocketView::SocketView(std::shared_ptr<const Socket> socket_p, const ViewResources & vRes_p)
    :DeviceView(socket_p),
     socket_m(socket_p),
     shape( SOCKET_SIZE )
{
}

void SocketView::Draw(sf::RenderTarget & rt)
{
    std::shared_ptr<const Socket> m {socket_m.lock()};
    if (m) {
        UpdatePos();
        shape.setPosition( actualPos + SOCKET_OFFSET );
        if (m->GetFiring()) shape.setFillColor(sf::Color::Green);
        else shape.setFillColor(sf::Color::Blue);
        rt.draw(shape);
    }
}