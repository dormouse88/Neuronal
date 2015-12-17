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
    ArenaPoint GetPos()                     { return pos; }
    virtual bool WhiskerDetectable() = 0;
    virtual void Draw(sf::RenderTarget & rt) = 0;
protected:
    ArenaPoint pos;
    Orientation ori;
    std::weak_ptr<Arena> arena;
};


class ArenaMotile : public ArenaStatic
{
public:
    ArenaMotile(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar);
    virtual void Act() = 0;
    virtual void Sense() = 0;
protected:
    void Forward();
    void Left();
    void Right();
    void Die();

    ArenaPoint GetPos(Orientation);
private:
    void BoundsCheck();
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
    void Draw(sf::RenderTarget & rt) override;
    bool WhiskerDetectable() override 		{ return true; }
//    bool WhiskerDetect()                        { ag->WhiskerDetect( GetTile(AdjacentTile::FRONT) ); }
//    bool Sniff()                                { ag->NoseDetect( GetTile(AdjacentTile::FRONT) ); }
};

class Hero : public Puppet
{
public:
    Hero(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar) :Puppet(s_pos, s_ori, ar) {}
    void Draw(sf::RenderTarget & rt) override;
    bool WhiskerDetectable() override 		{ return true; }
};


//Non-member Spawn Functions...
std::shared_ptr<Cat> SpawnCat(ArenaPoint p, Orientation o, std::shared_ptr<Arena> arena);
std::shared_ptr<Hero> SpawnHero(ArenaPoint p, Orientation o, std::shared_ptr<Arena> arena);


#endif	/* PUPPET_HPP */
