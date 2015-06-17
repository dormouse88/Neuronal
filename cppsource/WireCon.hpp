/* 
 * File:   WireCon.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 23:58
 */

#ifndef WIRECON_HPP
#define	WIRECON_HPP

#include <memory>
#include "GobjectCon.hpp"
#include "Wire.hpp"

class WireCon : public GobjectCon
{
public:
    WireCon(std::shared_ptr<Wire> wire_p);
    virtual ~WireCon() {}

    virtual void Handle(int code);
    virtual std::shared_ptr<const Wire> GetWire() const { return wire_m.lock(); }
    virtual bool IsDead() const { std::shared_ptr<const Wire> sh {wire_m.lock() } ; if (!sh) return true; return sh->IsDead();}
private:
    std::weak_ptr<Wire> wire_m;
};

#endif	/* WIRECON_HPP */

