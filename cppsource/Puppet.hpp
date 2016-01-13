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
#include "ArenaBasics.hpp"
class Arena;  //fwd dec


class ArenaEntity
{
public:
    ArenaEntity(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar);
    virtual bool WhiskerDetectable() = 0;
    virtual void Draw(sf::RenderTarget & rt) = 0;
    virtual void Act() = 0;
    virtual void Sense() = 0;

    ArenaPoint GetActualPos()                                             { return pos_; }
    Orientation GetActualOri()                                            { return ori_; }
    void Die();

protected:
    void WalkForward();
    void TurnLeft();
    void TurnRight();

    ArenaPoint GetAdjacentPos(Orientation);
    std::weak_ptr<Arena> arena_;
private:
    ArenaPoint pos_;
    Orientation ori_;
    bool alive_;
};



class Puppet : public ArenaEntity
{
public:
    Puppet(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar, std::shared_ptr<BaseReferer> brain);
    virtual void Act() override;
    virtual void Sense() override;

    //void SetBrain(std::shared_ptr<BaseReferer> b)       {brain_ = b;}
    std::shared_ptr<BaseReferer> GetBrain()             {return brain_;}
private:
    std::shared_ptr<BaseReferer> brain_;
};



const sf::Color MOUSE_COLOR_1   {210,140,140};
const sf::Color CAT_COLOR_1     {255,0,100};

class Mouse : public Puppet
{
public:
    Mouse(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar, std::shared_ptr<BaseReferer> brain) :Puppet(s_pos, s_ori, ar, brain) {}
    bool WhiskerDetectable()                override 	{ return true; }
    void Draw(sf::RenderTarget & rt)        override;
};

class Cat : public Puppet
{
public:
    Cat(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar, std::shared_ptr<BaseReferer> brain) :Puppet(s_pos, s_ori, ar, brain) {}
    bool WhiskerDetectable()                override    { return true; }
    void Draw(sf::RenderTarget & rt)        override;
};



#endif	/* PUPPET_HPP */
