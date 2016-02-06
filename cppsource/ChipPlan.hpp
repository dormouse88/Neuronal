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
#include "BasicTypes.hpp"
#include "PlanOwned.hpp"
#include "Wirable.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "PlanGrid.hpp"
class PlanRect; //fwd dec
#include "RefererInterface.hpp"
class UserData;  //fwd dec
class WiringPair; //fwd dec
class ChipHandle;

class ChipPlan : public Wirable
{
public:
    ChipPlan(std::shared_ptr<PlanGrid> g, std::shared_ptr<UserData> u);
    virtual ~ChipPlan() {}

    void RegisterReferer(std::shared_ptr<RefererInterface> ref);
    std::shared_ptr<RefererInterface> GetReferer();
    HandleShp GetHandle();

    //Wirable...
    virtual void ReCalculateCharge(Tag) override;
    virtual Charge GetOutgoingCharge(Tag) override;
    virtual VectorWorld GetWireAttachPos(WireAttachSide, Tag) const override;

    virtual bool IsSlotted(SlottedSide) const override      {return true;}
    virtual bool CanRegisterAnyWire(InOut side, Tag slot) const override;

    //"Referred"...
    void StepInReCalculateCharge(Tag slot);
    Charge StepInGetOutgoingCharge(Tag slot);
    void PassOnInnerStep();
    void PassOnPreInnerStep();


    void SetPosition(DeviceShp d, VectorSmart newPos);
    int GetFreeSerial() const;
    bool IsSerialFree(int serial) const;
    bool IsPositionFree(VectorSmart pos) const;
    int GetPlanID() const                                   {return planID;}

    void ImportDevice(DeviceShp device);
    void ImportWire(WireShp wire);
    void RemoveDevice(PlanPos pos); //DeviceShp device);
    void RemoveWire(WireShp wire); //Shp<WiringPair> wp);
    void CleanVectors();

    void SetModified();
    bool IsModified()                                       {return modified;}
    bool IsEmpty()                                          {return devices.empty() and wires.empty();}
    
    std::shared_ptr<PlanGrid> GetGrid()                     {return planGrid;}
    PlanRegion GetRegion(VectorSmart pos);
    PortLocation GetPort(VectorSmart pos);
    DeviceShp GetDevice(VectorSmart pos);
    DeviceShp GetDevice(int serial);
    WireShp GetWire(Shp<WiringPair> wp);
    std::vector<WireShp > GetWires(std::shared_ptr<Wirable>, bool from, bool to); 

    void RecalculateBounds();
    PlanRect GetSmartInnerBound() const;
    PlanRect GetSmartPaddedBound() const;
    RectDumb GetDumbPaddedBound() const;
    RectWorld GetWorldPaddedBound() const;
    RectWorld GetWorldPaddedBoundBox() const;
    RectWorld GetWorldPaddedBoundPlusPorts() const;
    RectWorld GetWorldPaddedBoundMinusPorts() const;
    
    void PlodeRefresh(VectorSmart point);
    void PlodeRefreshOutwards();
    
    void Draw(sf::RenderTarget & rt);
    void SubDraw(sf::RenderTarget & rt);

    VectorSmart GetPortSmartPos(ZoomSide, PortNum) const;
    void ReCalculatePorts(ZoomSide);
    PortNum MapTagToPort(ZoomSide, Tag) const;
    Tag MapPortToTag(ZoomSide, PortNum) const;
    Tag GetFirstFreeTag(ZoomSide);

private:
    void DrawBox(sf::RenderTarget & rt);
    void DrawTitle(sf::RenderTarget & rt);
    void DrawGridLines(sf::RenderTarget & rt);
    void DrawParts(sf::RenderTarget & rt);

    int planID;
    std::shared_ptr<const UserData> userData_;
    std::shared_ptr<PlanGrid> planGrid;
    std::shared_ptr<RefererInterface> referer;
    std::vector<DeviceShp > devices;
    std::vector<WireShp > wires;
    std::map<Tag, PortNum> inPorts_;
    std::map<Tag, PortNum> outPorts_;
    bool modified;
    VectorSmart inner_tl_;
    VectorSmart inner_br_;
    VectorSmart box_tl_;
    VectorSmart box_br_;
    
    friend class Serializer;
};


#endif	/* CHIPPLAN_HPP */

