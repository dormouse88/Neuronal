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
#include "PlanOwned.hpp"
#include "ViewResources.hpp"
class ChipPlan; //fwd dec
#include "PlanPos.hpp"



class Device : public Wirable, public PlanOwned
{
public:
    Device(int serial, VectorSmart pos, PlanShp cont);
    virtual ~Device() {}

    //virtuals...
    virtual void PreInnerStep() = 0;
    virtual void InnerStep() = 0;
    virtual VectorDumb GetPlodedSize() = 0;
    
    VectorWorld CalculateOffsetForCentering(VectorWorld objectSize) const;

    VectorWorld GetWorldPos() const             {return ppos_.GetWorldPos();}
    VectorWorld GetWorldSizeOfCell() const      {return ppos_.GetWorldSizeOf();}
    VectorSmart GetSmartPos() const             {return ppos_.GetSmartPos();}
    void SetPosSmart(VectorSmart p)             {ppos_.SetPosSmart(p);}

    int GetSerial() const                       {return serial_;}
private:
    PlanPos ppos_;
    int serial_;
    
};

#endif	/* DEVICE_HPP */

