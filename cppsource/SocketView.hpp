/* 
 * File:   SocketView.hpp
 * Author: Dormouse
 *
 * Created on 16 April 2015, 15:45
 */

#ifndef SOCKETVIEW_HPP
#define	SOCKETVIEW_HPP

#include <SFML/Graphics.hpp>
#include "Socket.hpp"
#include "ViewResources.hpp"
#include "DeviceView.hpp"

class SocketView : public DeviceView
{
public:
    SocketView(std::shared_ptr<const Socket> socket_p, const ViewResources & vRes_p);
    virtual ~SocketView() {}
    virtual void Draw(sf::RenderTarget & rt);
    //virtual bool IsDead() const {return socket_m.IsDead();}
private:
    std::weak_ptr<const Socket> socket_m;
    sf::CircleShape shape;
};

#endif	/* SOCKETVIEW_HPP */

