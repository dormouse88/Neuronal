/* 
 * File:   Socket.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:34
 */

#ifndef SOCKET_HPP
#define	SOCKET_HPP

#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Device.hpp"
class Socket;

class SocketView : public DeviceView
{
public:
    SocketView(const Socket & socket_p);
    virtual ~SocketView() {}
    void Draw(sf::RenderTarget & rt, const Socket & s);

private:
    sf::CircleShape shape;
};





class Socket : public Device
{
public:
    Socket(int serial, sf::Vector2i pos_p);
    virtual ~Socket() {;}

    virtual std::string SerialName() const { return "SOCK";}
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    virtual bool CanRegisterIn(int slot) const;
    
    virtual sf::Vector2f GetWireAttachPos(WireAttachSide) const;
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
private:
    SocketView v;
};

#endif	/* SOCKET_HPP */

