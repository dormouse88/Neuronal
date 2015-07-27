/* 
 * File:   ChipHandle.hpp
 * Author: Dormouse
 *
 * Created on 17 June 2015, 17:00
 */

#ifndef CHIPHANDLE_HPP
#define	CHIPHANDLE_HPP

#include <memory>
#include <string>

#include "Device.hpp"
#include "Wirable.hpp"
#include "ChipPlan.hpp"

class ChipHandle : public Device
{
public:
    ChipHandle(int serial_p, sf::Vector2i pos_p) :Device(serial_p, pos_p) {}
    virtual ~ChipHandle() {}
    virtual std::string SerialName() const { return "HAND";}
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    void StepOut(bool charge, int slot);
    virtual void LogicAct();
    virtual void LogicCalculate();

private:
    std::shared_ptr<ChipPlan> plan;
};

#endif	/* CHIPHANDLE_HPP */

