/* 
 * File:   Wirable.cpp
 * Author: Dormouse
 *
 * Created on 18 July 2015, 00:37
 */

#include "Wirable.hpp"

//bool Wirable::CanRegisterIn() const
//{
//    CleanWireVectors();
//    return inLimit  >  inWires.size() or  inLimit==-1 ? true : false;
//}
//bool Wirable::CanRegisterOut() const
//{
//    CleanWireVectors();
//    return outLimit > outWires.size() or outLimit==-1 ? true : false;
//}

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
            if (notDead->fromSlot == slot or slot == 0) {
                notDead->ReceiveCharge(firing);
            }
        }
    }
}

