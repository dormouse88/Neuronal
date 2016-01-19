/* 
 * File:   wire.hpp
 * Author: Dormouse
 *
 * Created on 13 January 2015, 16:09
 */

#ifndef WIRE_HPP
#define	WIRE_HPP

#include <SFML/Graphics.hpp>
#include "miscUtil.hpp"
#include "ViewResources.hpp"
#include "PlanOwned.hpp"
class Wirable;  //fwd dec

const int SLOT_MAX = 99;

class Wire;

class WireView
{
public:
    WireView(const Wire & wire_p);
    void Draw(sf::RenderTarget & rt, const Wire & w);
private:
    sf::Text weightText;
    sf::RectangleShape weightTextBoard;
    sf::Text fromSlotText;
    sf::RectangleShape fromSlotTextBoard;
    sf::Text toSlotText;
    sf::RectangleShape toSlotTextBoard;
};




class Wire : public PlanOwned
{
public:
    Wire(Wirable & from_p, int fromSlot_p, Wirable & to_p, int toSlot_p, signed weight_p, std::shared_ptr<ChipPlan> cont);

    virtual std::string SerialName() const { return "WIRE";}

    //void ReceiveCharge(bool f);
    void Refresh();
    int GetOutgoingWeight() const          { if (firing) return weight; else return 0;}
    
    bool GetFiring() const {return firing;}     //for drawing only
    int GetWeight() const {return weight;}      //for drawing only
    void SetWeight(int w) {weight = w;}         //for drawing only

    const Wirable& GetFrom() const {return from; }
    const int GetFromSlot() const {return fromSlot; }
    const Wirable& GetTo() const {return to; }
    const int GetToSlot() const {return toSlot; }
    
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;

    void SlotCycle(int step, bool fromSide);
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

