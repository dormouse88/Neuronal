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
    Wire(Wirable & from, Tag fromTag, Wirable & to, Tag toTag, signed weight, PlanShp cont);

    virtual std::string SerialName() const { return "WIRE";}

    void Refresh();
    int GetOutgoingWeight() const          { if (firing_) return weight_; else return 0;}
    
    bool GetFiring() const {return firing_;}     //for drawing only
    int GetWeight() const {return weight_;}      //for drawing only

    const Wirable& GetFrom() const {return from_; }
    const int GetFromTag() const {return fromTag_; }
    const Wirable& GetTo() const {return to_; }
    const int GetToTag() const {return toTag_; }
    
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(int code) override;

    void SetWeight(int w);
    void SlotCycle(int step, bool fromSide);
private:
    Wirable& from_;
    int fromTag_;
    Wirable& to_;
    int toTag_;
    signed weight_;
    bool firing_;
    WireView v_;
};

#endif	/* WIRE_HPP */

