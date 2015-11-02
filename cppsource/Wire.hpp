/* 
 * File:   wire.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:09
 */

#ifndef WIRE_HPP
#define	WIRE_HPP

#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
class Wire;

class WireView
{
public:
    WireView(const Wire & wire_p);
    void Draw(sf::RenderTarget & rt, const Wire & w);

private:
    sf::Text weightText;
    sf::Text fromSlotText;
    sf::Text toSlotText;
};





#include "Gobject.hpp"
class Wirable;

class Wire : public Gobject
{
public:
    Wire(Wirable & from_p, int fromSlot_p, Wirable & to_p, int toSlot_p, signed weight_p);

    virtual std::string SerialName() const { return "WIRE";}

    void ReceiveCharge(bool f);
    
    bool GetFiring() const {return firing;}
    int GetWeight() const {return weight;}
    void SetWeight(int w) {weight = w;}

    const Wirable& GetFrom() const {return from; }
    const int GetFromSlot() const {return fromSlot; }
    const Wirable& GetTo() const {return to; }
    const int GetToSlot() const {return toSlot; }
    
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;
private:
    Wirable& from;
    int fromSlot;
    Wirable& to;
    int toSlot;
    signed weight;
    bool firing;
    WireView v;
};

#endif	/* WIRE_HPP */

