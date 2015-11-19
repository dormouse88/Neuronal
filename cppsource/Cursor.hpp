/* 
 * File:   Cursor.hpp
 * Author: Dormouse
 *
 * Created on 18 January 2015, 20:37
 */

#ifndef CURSOR_HPP
#define	CURSOR_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "ViewResources.hpp"
class ChipPlan;
#include "PlanPos.hpp"

class Cursor
{
public:
    Cursor(std::shared_ptr<ChipPlan> p, sf::Color color = sf::Color::Yellow);
    void Draw(sf::RenderTarget & rt);

    PlanPos GetPlanPos() const;
    void Nullify()                                            {ppos.SetPlanOnly();}
    void SetPosWorld(VectorWorld);  //, std::shared_ptr<PlanGrid>
    
private:
    PlanPos ppos;
    sf::RectangleShape representation;
};


  //Backup class//
//class Cursor
//{
//public:
//    Cursor(sf::Color color = sf::Color::Yellow);
//    void Draw(sf::RenderTarget & rt);
//
//    std::shared_ptr<PlanPos> GetPlanPos() const;
//    //void SetPlanPos(std::shared_ptr<PlanPos> ppos)            {pposPtr = ppos;}
//
//    //void SetPosSmart(VectorSmart, std::shared_ptr<PlanGrid>);
//    //void SetPosWorld(VectorWorld, std::shared_ptr<PlanGrid>);
//
//    void Nullify()                                            {posPtr = nullptr;}
////    void SetPosPlanOnly(std::shared_ptr<ChipPlan> p)          { planPtr = p; posPtr = nullptr;}
////    void SetPosSmart(VectorSmart, std::shared_ptr<ChipPlan>);
//    void SetPosWorld(VectorWorld, std::shared_ptr<ChipPlan>);
//    
//private:
//    std::shared_ptr<ChipPlan> planPtr;
//    std::shared_ptr<VectorSmart> posPtr;
//    sf::RectangleShape representation;
//};


#endif	/* CURSOR_HPP */

