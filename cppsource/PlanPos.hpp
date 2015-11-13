/* 
 * File:   PlanPos.hpp
 * Author: Dormouse
 *
 * Created on 11 November 2015, 11:52
 */

#ifndef PLANPOS_HPP
#define	PLANPOS_HPP

#include <memory>
#include <SFML/System.hpp>  //for sf::Vector
#include <SFML/Graphics.hpp> //for sf::Rect
#include "Pansions.hpp"

typedef float World;

typedef sf::Vector2<Dumb> VectorDumb;
typedef sf::Vector2<Smart> VectorSmart;
typedef sf::Vector2<World> VectorWorld;

typedef sf::Rect<Dumb> RectDumb;
typedef sf::Rect<Smart> RectSmart;
typedef sf::Rect<World> RectWorld;

template <class T>
struct PairVector
{
    PairVector() :valid(false) {}
    PairVector(sf::Vector2<T> tl_, sf::Vector2<T> br_)
            :tl(tl_), br(br_), valid(true)
    {}
// Size is problematic across ints and floats...
//    sf::Rect<T> GetRect()
//    {
//        return valid ? sf::Rect<T> {tl, br-tl + sf::Vector2<T>{1,1} } : sf::Rect<T> { tl, sf::Vector2<T>{0,0} };
//    }
    PairVector<T> AddPadding(PairVector<T> & pvs, int thickness)
    {
        pvs.tl -= sf::Vector2i{ thickness, thickness };
        pvs.br += sf::Vector2i{ thickness, thickness };
        return *this;
    }

    sf::Vector2<T> tl;
    sf::Vector2<T> br;
    bool valid;
};



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
private:
    Pansions xPansions;
    Pansions yPansions;
};





class PlanPos
{
public:
    PlanPos(VectorSmart, std::shared_ptr<PlanGrid>);
    PlanPos(VectorWorld, std::shared_ptr<PlanGrid>);
    VectorSmart GetSmartPos() const;
    VectorWorld GetWorldPos() const;
    VectorWorld GetWorldSizeOf() const;
    void SetPlan(std::shared_ptr<PlanGrid>);
    void SetPos(VectorSmart);
    void SetPos(VectorWorld);
private:
    VectorSmart pos;
    std::shared_ptr<PlanGrid> planGrid;
};

#endif	/* PLANPOS_HPP */

