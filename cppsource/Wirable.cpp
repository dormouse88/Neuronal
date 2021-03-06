/* 
 * File:   Wirable.cpp
 * Author: Dormouse
 *
 * Created on 18 July 2015, 00:37
 */

#include "Wirable.hpp"
#include "Wire.hpp"


void Wirable::RegisterWire(InOut side, WireShp w)
{
    if(side == InOut::IN)
        inWires.emplace_back(w);
    else
        outWires.emplace_back(w);
}

bool Wirable::CanRegisterExactWire(Tag fromTag, Wirable & to, Tag toTag) const
{
    return not HasWireTo(fromTag, to, toTag) and
            CanRegisterAnyWire(InOut::OUT, fromTag) and
            to.CanRegisterAnyWire(InOut::IN, toTag) and
            this != &to;
}

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
    }
    return false;
}

int Wirable::CountWires(InOut side) const
{
    CleanWireVectors();
    if (side == InOut::IN)
        return inWires.size();
    else
        return outWires.size();
}

std::set<Tag> Wirable::GetTagCloud(InOut side)
{
    CleanWireVectors();
    std::set<Tag> ret;
    if (side == InOut::IN)
    {
        for (auto & x: inWires)
        {
            auto l = x.lock();
            if (l and not l->IsDead())
                ret.insert( l->GetToTag() );
        }
    }
    else
    {
        for (auto & x: outWires)
        {
            auto l = x.lock();
            if (l and not l->IsDead())
                ret.insert( l->GetFromTag() );
        }
    }
    return ret;
}

void Wirable::RenameTag(InOut side, Tag oldTag, Tag newTag)
{
    if (side == InOut::IN)
    {
        for (auto x : inWires)
        {
            auto locked = x.lock();
            if (locked and locked->GetToTag() == oldTag)
                locked->SetToTag(newTag);
        }
    }
    else
    {
        for (auto x : outWires)
        {
            auto locked = x.lock();
            if (locked and locked->GetFromTag() == oldTag)
                locked->SetFromTag(newTag);
        }
    }
}





//////////////////////////////////////////////////protected or private

void Wirable::PropagateRefresh(Tag tag)
{
    for (auto & w: outWires)
    {
        if (auto notDead = w.lock()) {
            if (notDead->GetFromTag() == tag or tag == NULL_TAG) {
                notDead->Refresh();
            }
        }
    }
}

CWeight Wirable::GetTotalIncomingWeight(Tag tag) const
{
    CleanWireVectors();
    CWeight total = 0;
    for (auto w: inWires)
    {
        auto lw = w.lock();
        if (lw and (lw->GetToTag() == tag or tag == NULL_TAG) )
        {
            total += lw->GetOutgoingWeight();
        }
    }
    return total;
}

void Wirable::CleanWireVectors() const //this method is necessary since the limits work by checking vector size.
{
    auto cleanVector = [&] (std::vector<std::weak_ptr<Wire> > & wv) {
        auto remove_func = [] (std::weak_ptr<Wire> & w)
        //{ return w.expired() or w.lock()->IsDead(); } ;
        {
            auto l = w.lock();
            if (l and not l->IsDead())
                return false;
            else
                return true;
        };
        auto new_end = std::remove_if(begin(wv), end(wv), remove_func );
        wv.erase(new_end, end(wv));
    };
    cleanVector(inWires);
    cleanVector(outWires);
}



//RED
//bool Wirable::IsTagFree(InOut side, Tag tag) const
//{
//    CleanWireVectors();
//    if (side == InOut::IN)
//    {
//        for (auto w: inWires)
//        {
//            if (w.lock()->GetToTag() == tag)
//                return false;
//        }
//    }
//    if (side == InOut::OUT)
//    {
//        for (auto w: outWires)
//        {
//            if (w.lock()->GetFromTag() == tag)
//                return false;
//        }
//    }
//    return true;
//}
