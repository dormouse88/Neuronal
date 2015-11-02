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
    virtual std::string SerialName() const override { return "HAND";}
    
    virtual void ReceiveCharge(bool charge, int weight, int slot) override;

    virtual bool IsSlotted(SlottedSide) const override;
    virtual bool CanRegisterIn(int slot) const override;
    virtual bool CanRegisterOut(int slot) const override;
    void StepOut(bool charge, int slot);
    virtual void LogicAct() override;
    virtual void LogicCalculate() override;
    
    void SetPlan(std::shared_ptr<ChipPlan> newPlan)     {plan = newPlan;}
    std::shared_ptr<ChipPlan> GetPlan()                 {return plan;}

    virtual sf::Vector2f GetWireAttachPos(WireAttachSide was) const override;
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
private:
    std::shared_ptr<ChipPlan> plan;
    sf::RectangleShape shape;
};

#endif	/* CHIPHANDLE_HPP */

