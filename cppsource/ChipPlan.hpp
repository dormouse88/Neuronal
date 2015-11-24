/* 
 * File:   ChipPlan.hpp
 * Author: Dormouse
 *
 * Created on 17 June 2015, 16:57
 */

#ifndef CHIPPLAN_HPP
#define	CHIPPLAN_HPP

#include <memory>
#include <vector>
#include "Gobject.hpp"
#include "Wirable.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "PlanGrid.hpp"
#include "PlanPos.hpp"
#include "RefererInterface.hpp"
class ChipHandle;

class Relatives; //kill with fire

const sf::Vector2f GRABBER_SIZE { GRID_SIZE * 0.5f };

class ChipPlan : public Wirable
{
public:
    ChipPlan(std::shared_ptr<PlanGrid> g);
    virtual ~ChipPlan() {}

    void RegisterReferer(std::shared_ptr<RefererInterface> ref);
    std::shared_ptr<RefererInterface> GetReferer();
    std::shared_ptr<ChipHandle> GetHandle();

    virtual std::string SerialName() const { return "PLAN";}
    virtual void ReceiveCharge(bool charge, int weight, int slot) override;
    virtual bool IsSlotted(SlottedSide) const override                                                  {return true;}
    virtual bool CanRegisterIn(int slot) const override;
    virtual bool CanRegisterOut(int slot) const override;
    
    void StepIn(bool charge, int slot);
    void PassOnAct();
    void PassOnCalculate();


    void SetPosition(Device & d, VectorSmart newPos);
    int GetFreeSerial() const;
    bool IsSerialFree(int serial) const;
    bool IsPositionFree(VectorSmart pos) const;
    int GetPlanID()                                                                                     {return planID;}

    void ImportDevice(std::shared_ptr<Device> device);
    void ImportWire(std::shared_ptr<Wire> wire);
    void RemoveDevice(std::shared_ptr<Device> device);
    void RemoveWire(std::shared_ptr<Wire> wire);
    void CleanVectors();

    void SetModified();
    bool IsModified()                                                                                   {return modified;}
    bool IsEmpty()                                                                                      {return devices.empty() and wires.empty();}
    
    std::shared_ptr<PlanGrid> GetGrid()                                                                 {return planGrid;}
    std::shared_ptr<Device> GetDevice(VectorSmart pos);
    std::shared_ptr<Device> GetDevice(int serial);
    std::shared_ptr<Wire> GetWire(const Wirable& from, const Wirable& to);
    std::vector<std::shared_ptr<Wire> > GetWires(std::shared_ptr<Wirable> device, bool from, bool to); 

    virtual VectorWorld GetWireAttachPos(WireAttachSide was) const override;
    
    void RecalculateSmartInnerBound();
    PlanRect GetSmartInnerBound() const;
    PlanRect GetSmartPaddedBound() const;
    RectDumb GetDumbPaddedBound() const;
    RectWorld GetWorldPaddedBound() const;
    
    void SetPadding(int thickness)                                                                      {padding = thickness;}
    void PlodeRefresh(VectorSmart point);
    void PlodeRefresh();
    
    void DrawBox(sf::RenderTarget & rt);
    void DrawTitle(sf::RenderTarget & rt);
    void DrawGridLines(sf::RenderTarget & rt);
    void DrawParts(sf::RenderTarget & rt);
    void SubDraw(sf::RenderTarget & rt);
    void Draw(sf::RenderTarget & rt);

private:
    int planID;
    std::function<std::string(int)> nameGetter;
    std::function<const Relatives&(int)> relativesGetter;
    std::shared_ptr<PlanGrid> planGrid;
    std::weak_ptr<RefererInterface> referer;
    std::vector<std::shared_ptr<Device> > devices;
    std::vector<std::shared_ptr<Wire> > wires;
    bool modified;
    int padding;
    PlanRect smartInnerBound;
    
    friend class Serializer;
};

namespace ChipPlanFunc
{
    std::shared_ptr<Device> GetDevice(PlanPos pos);
    std::shared_ptr<Wirable> GetWirable(PlanPos pos);
    std::shared_ptr<Wire> GetWire(PlanPos pos1, PlanPos pos2);
    bool IsPositionFree(PlanPos pos);
    void SetPosition(PlanPos dPos, PlanPos toPos);

    void DeviceHandle(PlanPos pos, int code);
    void WireHandle(PlanPos pos1, PlanPos pos2, int code);
    bool MatchOnPlan(PlanPos & pos1, PlanPos & pos2);
}




#endif	/* CHIPPLAN_HPP */

