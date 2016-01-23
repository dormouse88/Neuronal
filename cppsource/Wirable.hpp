/* 
 * File:   Wirable.hpp
 * Author: Dormouse
 *
 * Created on 18 July 2015, 00:29
 */

#ifndef WIRABLE_HPP
#define	WIRABLE_HPP

#include <memory>
#include <vector>
#include "BasicTypes.hpp"
#include "PlanGrid.hpp"     //for VectorWorld (for now)
class Wire;

enum class WireAttachSide { IN, OUT };
enum class SlottedSide { IN, OUT };

class Wirable
{
public:
    Wirable() {}
    virtual ~Wirable() {}

    //Virtuals...
    virtual void Refresh(Tag slot) = 0;
    virtual bool GetOutgoingCharge(Tag slot) = 0;
    virtual VectorWorld GetWireAttachPos(WireAttachSide) const = 0;

    virtual bool IsWeightedIn() const                   {return false;}
    virtual bool IsSlotted(SlottedSide) const           {return false;}
    virtual bool CanRegisterWire(InOut side, Tag slot) const          {return true;}  //Client needs to call this before calling RegisterWire(). RegisterWire() should throw an exception on failure perhaps.

    void RegisterWire(InOut side, WireShp w);
    bool HasWireTo(Tag fromSlot, Wirable & to, Tag toSlot) const;
    int CountWires(InOut side) const;
    bool IsTagFree(InOut side, Tag tag) const;
    
protected:
    void PropagateRefresh(Tag slot = 0);
    int GetTotalIncomingWeight(Tag slot = 0) const;
private:
    void CleanWireVectors() const;
    mutable std::vector<std::weak_ptr<Wire> > inWires;
    mutable std::vector<std::weak_ptr<Wire> > outWires;
};

#endif	/* WIRABLE_HPP */

