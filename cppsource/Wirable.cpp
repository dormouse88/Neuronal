/* 
 * File:   Wirable.cpp
 * Author: Dormouse
 *
 * Created on 18 July 2015, 00:37
 */

#include "Wirable.hpp"
#include "Wire.hpp"


bool Wirable::HasWireTo(Tag fromSlot, Wirable & to, Tag toSlot) const //slot parameters are now redundant
{
    for (const auto w: outWires) {
        auto wire = w.lock();
        if (wire) {
            //old version that allowed identically located wires with different slots...
            //if ( (wire->GetFromSlot() == fromSlot) and (&wire->GetTo() == &to) and (wire->GetToSlot() == toSlot) ) return true;
            if ( &wire->GetTo() == &to ) return true;
        }
    }
    return false;
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
            if (notDead->GetFromSlot() == slot or slot == 0) {
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
        if (lw and (lw->GetToSlot() == slot or slot == 0) )
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

//bool Wirable::IsInSlotFree(Tag slot) const
//{
//    CleanWireVectors();
//    for (auto w: inWires) {
//        if (w.lock()->GetToSlot() == slot) return false;
//    }
//    return true;
//}
//bool Wirable::IsOutSlotFree(Tag slot) const
//{
//    CleanWireVectors();
//    for (auto w: outWires) {
//        if (w.lock()->GetToSlot() == slot) return false;
//    }
//    return true;
//}

bool Wirable::IsTagFree(InOut side, Tag tag) const
{
    CleanWireVectors();
    if (side == InOut::IN)
    {
        for (auto w: inWires)
        {
            if (w.lock()->GetToSlot() == tag)
                return false;
        }
    }
    if (side == InOut::OUT)
    {
        for (auto w: outWires)
        {
            if (w.lock()->GetFromSlot() == tag)
                return false;
        }
    }
    return true;
}

