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
#include "PlanOwned.hpp"
#include "Wirable.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "PlanGrid.hpp"
#include "PlanPos.hpp"
#include "RefererInterface.hpp"
class UserData;  //fwd dec
class ChipHandle;

const sf::Vector2f GRABBER_SIZE { GRID_SIZE * 0.5f };

class ChipPlan : public Wirable
{
public:
    ChipPlan(std::shared_ptr<PlanGrid> g, std::shared_ptr<UserData> u);
    virtual ~ChipPlan() {}

    void RegisterReferer(std::shared_ptr<RefererInterface> ref);
    std::shared_ptr<RefererInterface> GetReferer();
    std::shared_ptr<ChipHandle> GetHandle();

    //Wirable...
    virtual void Refresh(int slot) override;
    virtual bool GetOutgoingCharge(int slot) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide was) const override;

    virtual bool IsSlotted(SlottedSide) const override      {return true;}
    virtual bool CanRegisterIn(int slot) const override;
    virtual bool CanRegisterOut(int slot) const override;

    //"Referred"...
    void StepInRefresh(int slot);
    bool StepInGetOutgoingCharge(int slot);
    void PassOnAct();
    void PassOnCalculate();


    void SetPosition(Device & d, VectorSmart newPos);
    int GetFreeSerial() const;
    bool IsSerialFree(int serial) const;
    bool IsPositionFree(VectorSmart pos) const;
    int GetPlanID() const                                   {return planID;}

    void ImportDevice(std::shared_ptr<Device> device);
    void ImportWire(std::shared_ptr<Wire> wire);
    void RemoveDevice(std::shared_ptr<Device> device);
    void RemoveWire(std::shared_ptr<Wire> wire);
    void CleanVectors();

    void SetModified();
    bool IsModified()                                       {return modified;}
    bool IsEmpty()                                          {return devices.empty() and wires.empty();}
    
    std::shared_ptr<PlanGrid> GetGrid()                     {return planGrid;}
    std::shared_ptr<Device> GetDevice(VectorSmart pos);
    std::shared_ptr<Device> GetDevice(int serial);
    std::shared_ptr<Wire> GetWire(const Wirable& from, const Wirable& to);
    std::vector<std::shared_ptr<Wire> > GetWires(std::shared_ptr<Wirable>, bool from, bool to); 

    void RecalculateSmartInnerBound();
    PlanRect GetSmartInnerBound() const;
    PlanRect GetSmartPaddedBound() const;
    RectDumb GetDumbPaddedBound() const;
    RectWorld GetWorldPaddedBound() const;
    RectWorld GetWorldPaddedBoundBox() const;
    RectWorld GetWorldPaddedBoundPlusPorts() const;
    RectWorld GetWorldPaddedBoundMinusPorts() const;
    
    void SetPadding(int thickness)                        {padding = thickness;}
    void PlodeRefresh(VectorSmart point);
    void PlodeRefreshOutwards();
    
    void DrawBox(sf::RenderTarget & rt);
    void DrawTitle(sf::RenderTarget & rt);
    void DrawGridLines(sf::RenderTarget & rt);
    void DrawParts(sf::RenderTarget & rt);
    void SubDraw(sf::RenderTarget & rt);
    void Draw(sf::RenderTarget & rt);

private:
    int planID;
    std::shared_ptr<const UserData> userData_;
    std::shared_ptr<PlanGrid> planGrid;
    std::shared_ptr<RefererInterface> referer;
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
    std::vector<std::shared_ptr<Wire> > GetWires(PlanPos pos, bool from, bool to);
    bool IsPositionFree(PlanPos pos);
    void SetPosition(PlanPos dPos, PlanPos toPos);

    void DeviceHandle(PlanPos pos, int code);
    void WireHandle(PlanPos pos1, PlanPos pos2, int code);
    bool MatchOnPlan(PlanPos & pos1, PlanPos & pos2);
}




#endif	/* CHIPPLAN_HPP */

