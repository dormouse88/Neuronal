/* 
 * File:   Socket.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:34
 */

#ifndef SOCKET_HPP
#define	SOCKET_HPP

#include "Device.hpp"

class Socket : public Device
{
public:
    Socket(sf::Vector2i pos_p);
    virtual ~Socket() {;}

    virtual bool IsInstant() {return true;}
    virtual void ReceiveCharge(int weight);
    virtual void PushCharge();
private:
};

#endif	/* SOCKET_HPP */

