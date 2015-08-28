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
#include "ChipPlan.hpp"

class ChipHandle : public Device, public DeviceView
{
public:
    ChipHandle(int serial_p, sf::Vector2i pos_p);
    virtual ~ChipHandle() {}
    virtual std::string SerialName() const { return "HAND";}
    
    virtual void ReceiveCharge(bool charge, int weight, int slot);
    void StepOut(bool charge, int slot);
    virtual void LogicAct();
    virtual void LogicCalculate();

    virtual sf::Vector2f GetWireAttachPos(WireAttachSide was) const override;
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
private:
    std::shared_ptr<ChipPlan> plan;
    sf::RectangleShape shape;
};

#endif	/* CHIPHANDLE_HPP */

