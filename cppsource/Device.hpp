/* 
 * File:   Device.hpp
 * Author: Dormouse
 *
 * Created on 06 April 2015, 19:09
 */

#ifndef DEVICE_HPP
#define	DEVICE_HPP

#include <memory>
#include <SFML/System.hpp>  //for sf::Vector
#include "Wirable.hpp"
#include "Gobject.hpp"
#include "ViewResources.hpp"
class ChipPlan; //fwd dec

class DeviceView
{
public:
    DeviceView(sf::Vector2f targetPos)
        {
            actualPos = targetPos;
        }
    virtual ~DeviceView() {}
protected:
    void UpdatePos(sf::Vector2f targetPos)
    {
        actualPos += (targetPos - actualPos) * 0.08f;
    }
    sf::Vector2f actualPos;
};




class Device : public Wirable, public Gobject
{
public:
    Device(int serial_p, sf::Vector2i pos_p, std::shared_ptr<ChipPlan> cont)
        :Gobject(cont), serial(serial_p), pos(pos_p)
        {}
    virtual ~Device() {}

    virtual void LogicAct() {}
    virtual void LogicCalculate() {}

    sf::Vector2f GetWorldPos() const;
    sf::Vector2i GetPosInPlan() const            {return pos;}
    void SetPosInPlan(sf::Vector2i p)            {pos = p;}
    int GetSerial() const                       {return serial;}
private:
    sf::Vector2i pos;
    int serial;
    
};

#endif	/* DEVICE_HPP */

