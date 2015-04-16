/* 
 * File:   WireCon.cpp
 * Author: Dormouse
 * 
 * Created on 14 April 2015, 23:58
 */

#include "WireCon.hpp"

WireCon::WireCon(Wire & wire_p)
    :wire_m(wire_p)
{}

void WireCon::Handle(int code)
{
    if (code == 1) { wire_m.SetWeight( wire_m.GetWeight() + 1); }
    if (code == 2) { wire_m.SetWeight( wire_m.GetWeight() - 1); }
}
