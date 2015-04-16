/* 
 * File:   WireCon.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 23:58
 */

#ifndef WIRECON_HPP
#define	WIRECON_HPP

#include "GobjectCon.hpp"
#include "Wire.hpp"

class WireCon : public GobjectCon
{
public:
    WireCon(Wire & wire_p);
    virtual ~WireCon() {}

    virtual void Handle(int code);
    virtual const Wire& GetWire() const { return wire_m; }
    virtual bool IsDead() const {return wire_m.IsDead();}
private:
    Wire & wire_m;
};

#endif	/* WIRECON_HPP */

