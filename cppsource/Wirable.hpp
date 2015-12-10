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
    virtual void ReceiveCharge(bool charge, int weight, int slot) = 0;
    virtual VectorWorld GetWireAttachPos(WireAttachSide) const = 0;
    virtual bool IsWeightedIn() const                   {return false;}
    virtual bool IsSlotted(SlottedSide) const           {return false;}
    virtual bool CanRegisterIn(int slot) const          {return true;}  //Client needs to call this before calling RegisterIn(). RegisterIn() should throw an exception on failure perhaps.
    virtual bool CanRegisterOut(int slot) const         {return true;}

    void RegisterIn(std::shared_ptr<Wire> w)    { inWires.push_back(w);}
    void RegisterOut(std::shared_ptr<Wire> w)   { outWires.push_back(w);}
    bool HasWireTo(int fromSlot, Wirable & to, int toSlot) const;

protected:
    void PushCharge(bool charge, int slot = 0);
    void CleanWireVectors() const;
    mutable std::vector<std::weak_ptr<Wire> > inWires;
    mutable std::vector<std::weak_ptr<Wire> > outWires;
private:


};

#endif	/* WIRABLE_HPP */

