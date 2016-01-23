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
class PlanRect; //fwd dec
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
    HandleShp GetHandle();

    //Wirable...
    virtual void Refresh(Tag slot) override;
    virtual bool GetOutgoingCharge(Tag slot) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide was) const override;

    virtual bool IsSlotted(SlottedSide) const override      {return true;}
    virtual bool CanRegisterIn(Tag slot) const override;
    virtual bool CanRegisterOut(Tag slot) const override;

    //"Referred"...
    void StepInRefresh(Tag slot);
    bool StepInGetOutgoingCharge(Tag slot);
    void PassOnAct();
    void PassOnCalculate();


    void SetPosition(DeviceShp d, VectorSmart newPos);
    int GetFreeSerial() const;
    bool IsSerialFree(int serial) const;
    bool IsPositionFree(VectorSmart pos) const;
    int GetPlanID() const                                   {return planID;}

    void ImportDevice(DeviceShp device);
    void ImportWire(WireShp wire);
    void RemoveDevice(DeviceShp device);
    void RemoveWire(WireShp wire);
    void CleanVectors();

    void SetModified();
    bool IsModified()                                       {return modified;}
    bool IsEmpty()                                          {return devices.empty() and wires.empty();}
    
    std::shared_ptr<PlanGrid> GetGrid()                     {return planGrid;}
    DeviceShp GetDevice(VectorSmart pos);
    DeviceShp GetDevice(int serial);
    WireShp GetWire(WirableShp from, WirableShp to);
    std::vector<WireShp > GetWires(std::shared_ptr<Wirable>, bool from, bool to); 

    void RecalculateSmartInnerBound();
    PlanRect GetSmartInnerBound() const;
    PlanRect GetSmartPaddedBound() const;
    RectDumb GetDumbPaddedBound() const;
    RectWorld GetWorldPaddedBound() const;
    RectWorld GetWorldPaddedBoundBox() const;
    RectWorld GetWorldPaddedBoundPlusPorts() const;
    RectWorld GetWorldPaddedBoundMinusPorts() const;
    
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
    std::vector<DeviceShp > devices;
    std::vector<WireShp > wires;
    bool modified;
    //PlanRect smartInnerBound;
    VectorSmart tl_corner;
    VectorSmart br_corner;
    
    friend class Serializer;
};

namespace ChipPlanFunc
{
//    DeviceShp GetDevice(PlanPos pos);

//    WireShp GetWire(PlanPos pos1, PlanPos pos2);
//    std::vector<WireShp > GetWires(PlanPos pos, bool from, bool to);

//    void SetPosition(PlanPos dPos, PlanPos toPos);
//
//    void DeviceHandle(PlanPos pos, int code);
//    void WireHandle(PlanPos pos1, PlanPos pos2, int code);
//    bool MatchOnPlan(PlanPos & pos1, PlanPos & pos2);
}




#endif	/* CHIPPLAN_HPP */

