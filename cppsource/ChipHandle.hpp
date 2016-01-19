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

class ChipHandle : public Device, public RefererInterface
{
public:
    ChipHandle(int serial_p, sf::Vector2i pos_p, std::shared_ptr<ChipPlan> cont);
    virtual ~ChipHandle() {}
    //PlanOwned...
    virtual std::string SerialName() const override { return "HAND";}
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
    
    //Wirable...
    virtual void Refresh(int slot) override;
    virtual bool GetOutgoingCharge(int slot) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide was) const override;

    virtual bool IsSlotted(SlottedSide) const override;
    virtual bool CanRegisterIn(int slot) const override;
    virtual bool CanRegisterOut(int slot) const override;
    
    //Device...
    virtual void InnerStep() override;
    virtual void PreInnerStep() override;
    virtual VectorDumb GetPlodedSize() override;

    //RefererInterface...
    virtual void StepOutRefresh(int slot) override;
    virtual bool StepOutGetOutgoingCharge(int slot) override;
    virtual void SetModified() override;
    virtual void SetSubPlan(std::shared_ptr<ChipPlan>, std::shared_ptr<RefererInterface>) override;
    virtual std::shared_ptr<ChipPlan> GetSubPlan() override;

    bool IsExploded();
    void SetExploded(bool yes);
private:
    std::shared_ptr<ChipPlan> plan;
    bool exploded;
    sf::RectangleShape shape;
    sf::Text planNumText;
};

#endif	/* CHIPHANDLE_HPP */

