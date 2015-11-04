/* 
 * File:   Wirable.cpp
 * Author: Dormouse
 *
 * Created on 18 July 2015, 00:37
 */

#include "Wirable.hpp"
#include "Wire.hpp"


bool Wirable::HasWireTo(int fromSlot, Wirable & to, int toSlot) const //slot parameters are now redundant
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

void Wirable::PushCharge(bool firing, int slot)
{
    for (auto & w: outWires)
    {
        if (auto notDead = w.lock()) {
            if (notDead->GetFromSlot() == slot or slot == 0) {
                notDead->ReceiveCharge(firing);
            }
        }
    }
}

