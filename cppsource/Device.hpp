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
#include "PlanPos.hpp"

class DeviceView
{
public:
    DeviceView(sf::Vector2f targetPos)
        {
            perceivedPos = targetPos;
        }
    virtual ~DeviceView() {}
protected:
    void UpdatePos(sf::Vector2f targetPos)
    {
        perceivedPos += (targetPos - perceivedPos) * 0.08f;
    }
    sf::Vector2f perceivedPos;
};




class Device : public Wirable, public Gobject
{
public:
    Device(int serial_p, sf::Vector2i pos_p, std::shared_ptr<ChipPlan> cont)
        :Gobject(cont), serial(serial_p), ppos(pos_p, cont)
        {}
    virtual ~Device() {}

    virtual void LogicAct() {}
    virtual void LogicCalculate() {}

    sf::Vector2f GetPFPos() const            {return ppos.GetPFPos();}
    sf::Vector2f GetPFSize() const           {return ppos.GetPFSize();}
    sf::Vector2i GetPIPos() const            {return ppos.GetPIPos();}
    sf::Vector2f CalculateOffset(sf::Vector2f objectSize) const
        { return sf::Vector2f { GetPFPos() + (GetPFSize() - objectSize)/2.f }; }
    void SetPIPos(sf::Vector2i p)            {ppos.SetPos(p);}
    int GetSerial() const                       {return serial;}
private:
    PlanPos ppos;
    int serial;
    
};

#endif	/* DEVICE_HPP */

