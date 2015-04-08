/* 
 * File:   Socket.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:34
 */

#ifndef SOCKET_HPP
#define	SOCKET_HPP

#include "PinDevice.hpp"

class Socket : public PinDevice
{
public:
    Socket(sf::Vector2i pos_p);
    virtual ~Socket() {;}
private:

};

#endif	/* SOCKET_HPP */

