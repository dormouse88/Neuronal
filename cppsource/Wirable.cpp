/* 
 * File:   Wirable.cpp
 * Author: Dormouse
 *
 * Created on 18 July 2015, 00:37
 */

#include "Wirable.hpp"
#include "Wire.hpp"


bool Wirable::HasWireTo(Tag fromSlot, Wirable & to, Tag toSlot) const
{
    for (const auto w: outWires) {
        auto wire = w.lock();
        if (wire)
        {
            //allows identically located wires with different slots...
            if (
                    (wire->GetFromTag() == fromSlot or not IsSlotted(SlottedSide::OUT))
                    and
                    (&wire->GetTo() == &to)
                    and
                    (wire->GetToTag() == toSlot or not to.IsSlotted(SlottedSide::IN))
            )
                return true;
        }
//        {
//            //disallows identically located wires with different slots...
//            if ( &wire->GetTo() == &to )
//                return true;
//        }
    }
    return false;
}

bool Wirable::CanRegisterExactWire(Tag fromTag, Wirable & to, Tag toTag) const
{
    return not HasWireTo(fromTag, to, toTag) and
            CanRegisterAnyWire(InOut::OUT, fromTag) and
            to.CanRegisterAnyWire(InOut::IN, toTag) and
            this != &to;
}

void Wirable::CleanWireVectors() const //this method is necessary since the limits work by checking vector size.
{
    auto cleanVector = [&] (std::vector<std::weak_ptr<Wire> > & wv) {
        auto remove_func = [] (std::weak_ptr<Wire> & w) { return w.expired(); } ;
        auto new_end = std::remove_if(begin(wv), end(wv), remove_func );
        wv.erase(new_end, end(wv));
    };
    cleanVector(inWires);
    cleanVector(outWires);
}

void Wirable::PropagateRefresh(Tag slot)
{
    for (auto & w: outWires)
    {
        if (auto notDead = w.lock()) {
            if (notDead->GetFromTag() == slot or slot == 0) {
                notDead->Refresh();
            }
        }
    }
}

int Wirable::GetTotalIncomingWeight(Tag slot) const
{
    CleanWireVectors();
    int total = 0;
    for (auto w: inWires)
    {
        auto lw = w.lock();
        if (lw and (lw->GetToTag() == slot or slot == 0) )
        {
            total += lw->GetOutgoingWeight();
        }
    }
    return total;
}

void Wirable::RegisterWire(InOut side, WireShp w)
{
    if(side == InOut::IN)
        inWires.emplace_back(w);
    else
        outWires.emplace_back(w);
}


int Wirable::CountWires(InOut side) const
{
    CleanWireVectors();
    if (side == InOut::IN)
        return inWires.size();
    else
        return outWires.size();
}

bool Wirable::IsTagFree(InOut side, Tag tag) const
{
    CleanWireVectors();
    if (side == InOut::IN)
    {
        for (auto w: inWires)
        {
            if (w.lock()->GetToTag() == tag)
                return false;
        }
    }
    if (side == InOut::OUT)
    {
        for (auto w: outWires)
        {
            if (w.lock()->GetFromTag() == tag)
                return false;
        }
    }
    return true;
}

std::set<Tag> Wirable::GetTagCloud(InOut side)
{
    CleanWireVectors();
    std::set<Tag> ret;
    if (side == InOut::IN)
    {
        for (auto & x: inWires)
        {
            ret.insert( x.lock()->GetToTag() );
        }
    }
    else
    {
        for (auto & x: outWires)
        {
            ret.insert( x.lock()->GetFromTag() );
        }
    }
    return ret;
}
