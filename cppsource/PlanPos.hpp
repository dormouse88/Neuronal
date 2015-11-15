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

//template <class T>
//struct PairVector
//{
//    PairVector() :valid(false) {}
//    PairVector(sf::Vector2<T> tl_, sf::Vector2<T> br_)
//            :tl(tl_), br(br_), valid(true)
//    {}
//// Size is problematic across ints and floats...
////    sf::Rect<T> GetRect()
////    {
////        return valid ? sf::Rect<T> {tl, br-tl + sf::Vector2<T>{1,1} } : sf::Rect<T> { tl, sf::Vector2<T>{0,0} };
////    }
//    PairVector<T> AddPadding(PairVector<T> & pvs, int thickness)
//    {
//        pvs.tl -= sf::Vector2i{ thickness, thickness };
//        pvs.br += sf::Vector2i{ thickness, thickness };
//        return *this;
//    }
//
//    sf::Vector2<T> tl;
//    sf::Vector2<T> br;
//    bool valid;
//};


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
private:
    VectorWorld offset;
    Pansions xPansions;
    Pansions yPansions;
};





class PlanPos
{
public:
    PlanPos();
    PlanPos(VectorSmart, std::shared_ptr<PlanGrid>);
    PlanPos(VectorWorld, std::shared_ptr<PlanGrid>);

    void SetGrid(std::shared_ptr<PlanGrid>);
    bool IsValid() const;

    void SetPosSmart(VectorSmart);
    void SetPosDumb(VectorDumb);
    void SetPosWorld(VectorWorld);

    VectorSmart GetSmartPos() const;
    VectorDumb GetDumbPos() const;
    VectorWorld GetWorldPos() const;
    
    VectorDumb GetDumbSizeOf() const;
    VectorWorld GetWorldSizeOf() const;
private:
    VectorSmart pos;
    std::shared_ptr<PlanGrid> planGrid;
};

struct PlanRect
{
    PlanRect() :valid(false) {}
    PlanRect(PlanPos tl_, PlanPos br_)
            :tl(tl_), br(br_), valid(true) {}
    PlanRect AddPadding(int thickness)
    {
        tl.SetPosSmart( tl.GetSmartPos() - sf::Vector2i{ thickness, thickness } );
        br.SetPosSmart( br.GetSmartPos() + sf::Vector2i{ thickness, thickness } );
        return *this;
    }
    RectDumb GetRectDumb() const
    {
        return RectDumb {tl.GetDumbPos(), br.GetDumbPos() - tl.GetDumbPos() + br.GetDumbSizeOf() };
    }
    RectWorld GetRectWorld() const
    {
        return RectWorld {tl.GetWorldPos(), br.GetWorldPos() - tl.GetWorldPos() + br.GetWorldSizeOf() };
    }
    void SetGrid(std::shared_ptr<PlanGrid> newGrid) { tl.SetGrid(newGrid); br.SetGrid(newGrid); }
    PlanPos tl;
    PlanPos br;
    bool valid;
};


#endif	/* PLANPOS_HPP */

