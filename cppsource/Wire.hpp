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
#include "Wirable.hpp"
class Wirable;  //fwd dec

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

    void Refresh();
    CWeight GetOutgoingWeight() const          { if (charge_ == Charge::ON and not IsDead()) return weight_; else return 0; }
    
    bool GetFiring() const {return charge_ == Charge::ON ;}     //for drawing only
    CWeight GetWeight() const {return weight_;}      //for drawing only

    const Wirable& GetFrom() const {return from_; }
    const Tag GetFromTag() const {return fromTag_; }
    const Wirable& GetTo() const {return to_; }
    const Tag GetToTag() const {return toTag_; }
    
    virtual std::string SerialName() const { return "WIRE";}
    virtual void Draw(sf::RenderTarget & rt) override;
    virtual void Handle(HandleCode code) override;

    void SetWeight(CWeight w);
    void TagCycle(int step, bool fromSide);
private:
    Wirable& from_;
    Tag fromTag_;
    Wirable& to_;
    Tag toTag_;
    signed weight_;
    Charge charge_;
    WireView v_;
};

#endif	/* WIRE_HPP */

