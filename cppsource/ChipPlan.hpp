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
class ChipHandle;
#include "Device.hpp"
#include "Wire.hpp"
#include "PlanPos.hpp"

class ChipPlan : public Wirable
{
public:
    ChipPlan();
    void RegisterReferer(std::shared_ptr<ChipHandle> handle) { referer = handle; }
    std::shared_ptr<ChipHandle> GetReferer() {return referer.lock();}
    virtual ~ChipPlan() {}

    virtual std::string SerialName() const { return "PLAN";}
    virtual void ReceiveCharge(bool charge, int weight, int slot) override;
    virtual bool IsSlotted(SlottedSide) const override              {return true;}
    virtual bool CanRegisterIn(int slot) const override;
    virtual bool CanRegisterOut(int slot) const override;
    
    void StepIn(bool charge, int slot);
    void PassOnAct();
    void PassOnCalculate();


    void SetPosition(Device & d, sf::Vector2i newPos);
    int GetFreeSerial() const;
    bool IsSerialFree(int serial) const;
    bool IsPositionFree(sf::Vector2i pos) const;
    int GetPlanID() {return planID;}

    void ImportDevice(std::shared_ptr<Device> device);
    void ImportWire(std::shared_ptr<Wire> wire);
    void RemoveDevice(std::shared_ptr<Device> device);
    void RemoveWire(std::shared_ptr<Wire> wire);
    void CleanVectors();

    void SetModified();
    bool IsModified() {return modified;}
    bool IsEmpty() {return devices.empty() and wires.empty();}
    
    std::shared_ptr<PlanGrid> GetGrid() {return planGrid;}
    std::shared_ptr<Device> GetDevice(VectorSmart pos);
    std::shared_ptr<Device> GetDevice(int serial);
    std::shared_ptr<Wire> GetWire(const Wirable& from, const Wirable& to);
    std::vector<std::shared_ptr<Wire> > GetWires(std::shared_ptr<Wirable> device, bool from, bool to); 

    virtual VectorWorld GetWireAttachPos(WireAttachSide was) const override;
    
    PlanRect GetSmartInnerBound() const;  //used internally by getdumbsize and getworldpaddedbound
//    VectorDumb GetDumbSize(int thickness) const; //for get ploded size
    //PairVector<Dumb> GetDumbPaddedBound(int thickness) const;
//    RectWorld GetWorldPaddedBound(int thickness) const;  //for cursor and view clamping and handle view setting

    RectDumb GetDumbBound() const
    {
        auto pr = GetSmartInnerBound();
        return pr.AddPadding(padding).GetRectDumb();
        //else return RectDumb {-padding,-padding,padding*2,padding*2};
    }
    RectWorld GetWorldBound() const
    {
        auto pr = GetSmartInnerBound();
        return pr.AddPadding(padding).GetRectWorld();
    }
    
    void SetPadding(int thickness) {padding = thickness;}
    void PlodeRefresh(VectorSmart point);
    
    void DrawParts(sf::RenderTarget & rt);
    void SubDraw(sf::RenderTarget & rt);
    void Draw(sf::RenderTarget & rt);

private:
    int planID;
    std::shared_ptr<PlanGrid> planGrid;
    std::weak_ptr<ChipHandle> referer;  //need to be able to pass charge back out
    std::vector<std::shared_ptr<Device> > devices;
    std::vector<std::shared_ptr<Wire> > wires;
    bool modified;
    int padding;
    
    friend class Serializer;
};
#endif	/* CHIPPLAN_HPP */

