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
    Device(int serial_p, VectorSmart pos_p, std::shared_ptr<ChipPlan> cont);
    virtual ~Device() {}

    //virtuals...
    virtual void LogicAct() {}
    virtual void LogicCalculate() {}
    virtual VectorDumb GetPlodedSize()          { return VectorDumb {1, 1}; }
    
    VectorWorld CalculateOffset(VectorWorld objectSize) const;

    VectorWorld GetWorldPos() const             {return ppos.GetWorldPos();}
    VectorWorld GetWorldSizeOfCell() const      {return ppos.GetWorldSizeOf();}
    VectorSmart GetSmartPos() const             {return ppos.GetSmartPos();}
    void SetPosSmart(VectorSmart p)             {ppos.SetPosSmart(p);}

    int GetSerial() const                       {return serial;}
private:
    PlanPos ppos;
    int serial;
    
};

#endif	/* DEVICE_HPP */

