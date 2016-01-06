/* 
 * File:   Puppet.hpp
 * Author: Dormouse
 *
 * Created on 11 December 2015, 14:05
 */

#ifndef PUPPET_HPP
#define	PUPPET_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "BaseReferer.hpp"
#include "BlobFactory.hpp"
#include "Arena.hpp"


class ArenaStatic
{
public:
    ArenaStatic(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar);
    //pure virtuals...
    virtual bool WhiskerDetectable() = 0;
    virtual void Draw(sf::RenderTarget & rt) = 0;

    ArenaPoint GetInitialPos()                                                  { return initial_pos; }
    Orientation GetInitialOri()                                                  { return initial_ori; }
    virtual ArenaPoint GetActualPos()                                           { return initial_pos; }
    virtual Orientation GetActualOri()                                           { return initial_ori; }
protected:
    std::weak_ptr<Arena> arena;
private:
    const ArenaPoint initial_pos;
    const Orientation initial_ori;
};



class ArenaMotile : public ArenaStatic
{
public:
    ArenaMotile(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar);
    virtual void Act() = 0;
    virtual void Sense() = 0;
    virtual PredationStatus GetPredationStatus() = 0;

    ArenaPoint GetActualPos() final                                             { return actual_pos; }
    Orientation GetActualOri() final                                             { return actual_ori; }
    void Die();

protected:
    void WalkForward();
    void TurnLeft();
    void TurnRight();

    ArenaPoint GetAdjacentPos(Orientation);
private:
    ArenaPoint actual_pos;
    Orientation actual_ori;
};



class Puppet : public ArenaMotile
{
public:
    Puppet(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar);
    virtual void Act() override;
    virtual void Sense() override;
    
    std::shared_ptr<BaseReferer> GetBR()            {return inner;}
private:
    void InitBrain();
    std::shared_ptr<BaseReferer> inner;
};



class Cat : public Puppet
{
public:
    Cat(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar) :Puppet(s_pos, s_ori, ar) {}
    bool WhiskerDetectable()                override    { return true; }
    void Draw(sf::RenderTarget & rt)        override;
    PredationStatus GetPredationStatus()    override    { return PredationStatus::CAT; }
};

class Hero : public Puppet
{
public:
    Hero(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar) :Puppet(s_pos, s_ori, ar) {}
    bool WhiskerDetectable()                override 	{ return true; }
    void Draw(sf::RenderTarget & rt)        override;
    PredationStatus GetPredationStatus()    override    { return PredationStatus::MOUSE; }
};




//Non-member Spawn Functions...
std::shared_ptr<Cat> SpawnCat(ArenaPoint p, Orientation o, std::shared_ptr<Arena> arena);
std::shared_ptr<Hero> SpawnHero(ArenaPoint p, Orientation o, std::shared_ptr<Arena> arena);


#endif	/* PUPPET_HPP */
