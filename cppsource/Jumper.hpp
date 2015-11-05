/* 
 * File:   Jumper.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:34
 */

#ifndef JUMPER_HPP
#define	JUMPER_HPP

#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
#include "Device.hpp"
class Jumper;

class JumperView : public DeviceView
{
public:
    JumperView(const Jumper & socket_p);
    virtual ~JumperView() {}
    void Draw(sf::RenderTarget & rt, const Jumper & s);

private:
    sf::CircleShape shape;
};





class Jumper : public Device
{
public:
    Jumper(int serial, sf::Vector2i pos_p);
    virtual ~Jumper() {;}

    virtual std::string SerialName() const { return "JUMP";}
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    virtual bool CanRegisterIn(int slot) const;
    
    virtual sf::Vector2f GetWireAttachPos(WireAttachSide) const;
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
private:
    JumperView v;
};

#endif	/* JUMPER_HPP */

