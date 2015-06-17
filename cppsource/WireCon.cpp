/* 
 * File:   WireCon.cpp
 * Author: Dormouse
 * 
 * Created on 14 April 2015, 23:58
 */

#include "WireCon.hpp"

WireCon::WireCon(std::shared_ptr<Wire> wire_p)
    :wire_m(wire_p)
{}

void WireCon::Handle(int code)
{
    std::shared_ptr<Wire> m {wire_m.lock()};
    if (m) {
        if (code == 1) { m->SetWeight( m->GetWeight() + 1); }
        if (code == 2) { m->SetWeight( m->GetWeight() - 1); }
    }
}
