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

    //Pure Virtuals...
    virtual void StructuralRefresh() = 0;
    virtual void ReCalculateCharge(Tag) = 0;
    virtual Charge GetOutgoingCharge(Tag) = 0;
    virtual VectorWorld GetWireAttachPos(WireAttachSide, Tag) const = 0;

    //Virtuals...
    virtual bool IsWeightedIn() const                   {return false;}
    virtual bool IsSlotted(SlottedSide) const           {return false;}
    virtual Tag GetFirstFreeTag(InOut)                  {}
    virtual bool CanRegisterAnyWire(InOut side, Tag slot) const          {return true;}  //Client needs to call this before calling RegisterWire(). RegisterWire() should throw an exception on failure perhaps.

    //Concrete...
    void RegisterWire(InOut side, WireShp w);
    bool CanRegisterExactWire(Tag fromSlot, Wirable & to, Tag toSlot) const;
    bool HasWireTo(Tag fromSlot, Wirable & to, Tag toSlot) const;
    int CountWires(InOut side) const;
    std::set<Tag> GetTagCloud(InOut side);
    void RenameTag(InOut side, Tag oldTag, Tag newTag);
    
protected:
    void PropagateRefresh(Tag slot = NULL_TAG);
    CWeight GetTotalIncomingWeight(Tag) const;
private:
    void CleanWireVectors() const;
    mutable std::vector<std::weak_ptr<Wire> > inWires;
    mutable std::vector<std::weak_ptr<Wire> > outWires;
};

    //RED //bool IsTagFree(InOut side, Tag tag) const;

#endif	/* WIRABLE_HPP */

