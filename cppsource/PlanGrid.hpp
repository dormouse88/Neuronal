/* 
 * File:   PlanGrid.hpp
 * Author: Dormouse
 *
 * Created on 15 November 2015, 16:37
 */

#ifndef PLANGRID_HPP
#define	PLANGRID_HPP

#include <memory>
#include <SFML/System.hpp>  //for sf::Vector
#include <SFML/Graphics.hpp> //for sf::Rect
#include "Pansions.hpp"
class ChipPlan; //fwd dec only

typedef float World;

typedef sf::Vector2<Dumb> VectorDumb;
typedef sf::Vector2<Smart> VectorSmart;
typedef sf::Vector2<World> VectorWorld;

typedef sf::Rect<Dumb> RectDumb;
typedef sf::Rect<Smart> RectSmart;
typedef sf::Rect<World> RectWorld;


class PlanGrid
{
public:
    //smart <-> dumb <-> world <-> screen
    VectorDumb MapSmartToDumb(VectorSmart) const;
    VectorSmart MapDumbToSmart(VectorDumb) const;

    VectorWorld MapDumbToWorld(VectorDumb) const;
    VectorDumb MapWorldToDumb(VectorWorld) const;
    
    VectorWorld MapSmartToWorld(VectorSmart) const;
    VectorSmart MapWorldtoSmart(VectorWorld) const;

    VectorDumb DumbSizeOf(VectorSmart) const;
    VectorWorld WorldSizeOf(VectorSmart) const;

    void SetSizeX(Smart loc, Dumb size)     { xPansions.SetSize(loc, size); }
    void SetSizeY(Smart loc, Dumb size)     { yPansions.SetSize(loc, size); }
    
    void SetOffset(VectorWorld o)           { offset = o; }
    VectorWorld GetOffset() const           {return offset;}

    void RegisterPlan(std::shared_ptr<ChipPlan> p) {owner = p;}
    std::shared_ptr<ChipPlan> GetPlan() {return owner.lock();}
private:
    VectorWorld offset;
    Pansions xPansions;
    Pansions yPansions;
    std::weak_ptr<ChipPlan> owner;
};



#endif	/* PLANGRID_HPP */

