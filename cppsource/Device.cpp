/* 
 * File:   Device.cpp
 * Author: Dormouse
 *
 * Created on 24 April 2015, 15:09
 */

#include "Device.hpp"

bool Device::CanAcceptInput() const
{
    CleanWireVectors();
    return inLimit  >  inWires.size() or  inLimit==-1 ? true : false;
}
bool Device::CanAcceptOutput() const
{
    CleanWireVectors();
    return outLimit > outWires.size() or outLimit==-1 ? true : false;
}

void Device::CleanWireVectors() const //this method is necessary since the limits work by checking vector size.
{
    auto cleanVector = [&] (std::vector<std::weak_ptr<Wire> > & wv) {
        auto remove_func = [] (std::weak_ptr<Wire> & w) { return w.expired(); } ;
        auto new_end = std::remove_if(begin(wv), end(wv), remove_func );
        wv.erase(new_end, end(wv));
    };
    cleanVector(inWires);
    cleanVector(outWires);
}
