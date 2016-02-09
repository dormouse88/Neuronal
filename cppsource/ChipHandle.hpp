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
    ChipHandle(PlanID, VectorSmart, PlanShp cont);
    virtual ~ChipHandle() {}
    //PlanOwned...
    virtual std::string SerialName() const override { return "HAND";}
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
    
    //Wirable...
    virtual void StructuralRefresh() override;
    virtual void ReCalculateCharge(Tag) override;
    virtual Charge GetOutgoingCharge(Tag) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide, Tag) const override;

    virtual bool IsSlotted(SlottedSide) const override;
    virtual Tag GetFirstFreeTag(InOut);
    virtual bool CanRegisterAnyWire(InOut side, Tag slot) const override;
    
    //Device...
    virtual void InnerStep() override;
    virtual void PreInnerStep() override;
    virtual VectorDumb GetPlodedSize() override;

    //RefererInterface...
    virtual void StepOutReCalculateCharge(Tag slot) override;
    virtual Charge StepOutGetOutgoingCharge(Tag slot) override;
    virtual void SetModified() override;
    virtual void SetSubPlan(PlanShp, std::shared_ptr<RefererInterface>) override;
    virtual PlanShp GetSubPlan() override;

    bool IsExploded();
    void SetExploded(bool);
private:
    PlanShp subPlan_;
    bool exploded_;
    sf::RectangleShape shape_;
    sf::Text planNumText_;
};

#endif	/* CHIPHANDLE_HPP */

