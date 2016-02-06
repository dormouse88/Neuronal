/* 
 * File:   PlanOwned.hpp
 * Author: Dormouse
 *
 * Created on 09 November 2015, 16:52
 */

#ifndef PLANOWNED_HPP
#define	PLANOWNED_HPP

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "BasicTypes.hpp"
class ChipPlan;  //fwd dec

class PlanOwned
{
public:
    PlanOwned(PlanShp cont)
        :container(cont)
        , dead(false)
    {}
    virtual ~PlanOwned() = 0;
    virtual void Draw(sf::RenderTarget & rt) = 0;
    virtual void Handle(int code) = 0;
    virtual std::string SerialName() const = 0;

    void Zingaya()          {dead = true;}
    bool IsDead() const     {return dead;}
    //void SetModified();
    PlanShp GetContainer();
private:
    bool dead;
    std::weak_ptr<ChipPlan> container;
};

inline PlanOwned::~PlanOwned() {}


#endif	/* PLANOWNED_HPP */

