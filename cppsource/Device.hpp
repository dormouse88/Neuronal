/* 
 * File:   Device.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:09
 */

#ifndef DEVICE_HPP
#define	DEVICE_HPP

#include <memory>
#include <vector>
#include <string>
#include <SFML/System.hpp>  //for sf::Vector
#include "Wirable.hpp"

class Device : public Wirable
{
public:
    Device(int serial_p, sf::Vector2i pos_p)
            :pos(pos_p), serial(serial_p) {}
    virtual ~Device() {}
    bool operator==(const Device& rhs) const    { return this == &rhs; }

    virtual std::string SerialName() const = 0;
    virtual void LogicAct() {}
    virtual void LogicCalculate() {}

    sf::Vector2i GetPosition() const            {return pos;}
    void SetPosition(sf::Vector2i p)            {pos = p;}
    int GetSerial() const                      {return serial;}
private:
    int serial;
    sf::Vector2i pos;
};

#endif	/* DEVICE_HPP */

