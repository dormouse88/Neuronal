/* 
 * File:   Gobject.hpp
 * Author: Dormouse
 *
 * Created on 09 November 2015, 16:52
 */

#ifndef GOBJECT_HPP
#define	GOBJECT_HPP

#include <memory>
#include <SFML/Graphics.hpp>
class ChipPlan;  //fwd dec

class Drawable
{
public:
    virtual ~Drawable() {}
    virtual void Draw(sf::RenderTarget & rt) = 0;
};

class Responsive
{
public:
    virtual ~Responsive() {}
    virtual void Handle(int code) = 0;
};


#include <string>

class PlanOwned: public Drawable, public Responsive
{
public:
    PlanOwned(std::shared_ptr<ChipPlan> cont)
        :container(cont), dead(false)
        
    {}
    virtual ~PlanOwned() = 0;
    virtual std::string SerialName() const = 0;
    void Zingaya() {dead = true;}
    bool IsDead() const {return dead;}
    //void SetModified();
    std::shared_ptr<ChipPlan> GetContainer();
private:
    bool dead;
    std::weak_ptr<ChipPlan> container;
};

inline PlanOwned::~PlanOwned() {}


#endif	/* GOBJECT_HPP */

