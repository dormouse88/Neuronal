/* 
 * File:   Socket.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:34
 */

#ifndef SOCKET_HPP
#define	SOCKET_HPP

#include "Device.hpp"
#include "Wirable.hpp"

class Socket : public Device
{
public:
    Socket(int serial, sf::Vector2i pos_p);
    virtual ~Socket() {;}

    virtual std::string SerialName() const { return "SOCK";}
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    virtual bool CanRegisterIn(int slot) const;
private:
};

#endif	/* SOCKET_HPP */

