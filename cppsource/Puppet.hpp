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
//#include "Arena.hpp"
class Arena;  //fwd dec

//senses
//const std::string F_WHISK = "F_WHISK";
//const std::string L_WHISK = "L_WHISK";
//const std::string R_WHISK = "R_WHISK";
const SlotData F_WHISK {1,"F_WHISK",false};
const SlotData L_WHISK {2,"L_WHISK",false};
const SlotData R_WHISK {3,"R_WHISK",false};

//actions
//const std::string L_FOOT = "L_FOOT";
//const std::string R_FOOT = "R_FOOT";
const SlotData L_FOOT {1,"L_FOOT",false};
const SlotData R_FOOT {2,"R_FOOT",false};


inline XPuts GetMouseXPuts()
{
    XPuts ret;
    ret.ins.emplace_back(R_WHISK);
    ret.ins.emplace_back(F_WHISK);
    ret.outs.emplace_back(L_FOOT);
    ret.outs.emplace_back(R_FOOT);
    return ret;
}




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



/////////////////////////////////////Spawners

struct MouseSpawner
{
    MouseSpawner(ArenaPoint s_pos, Orientation s_ori)
        :pos_(s_pos)
        ,ori_(s_ori)
    {}
    ArenaPoint pos_;
    Orientation ori_;
};

struct CatSpawner
{
    CatSpawner(ArenaPoint s_pos, Orientation s_ori, TimeRange timeRange, int planNum)
        :pos_(s_pos)
        ,ori_(s_ori)
        ,timeRange_(timeRange)
        ,timeExact_(timeRange.start)
        ,planNum_(planNum)
        ,brain_(BlobFactory::MakeBrain())
    {
        brain_->DefineXputs(GetMouseXPuts());
        //cat needs to have the planNum plan loaded into it here!!
        //planNum_...
    }
    ArenaPoint pos_;
    Orientation ori_;
    TimeRange timeRange_;
    TimeExact timeExact_;
    int planNum_;
    std::shared_ptr<BaseReferer> brain_;
};



/////////////////////////////////////SpawnGroups
class SpawnGroup
{
public:
    virtual void Specify() = 0;
    virtual void DeSpecify() = 0;
    virtual void TimeIsNow(int t, std::shared_ptr<Arena> arena) = 0;
};

class MouseSpawnGroup : public SpawnGroup
{
public:
    MouseSpawnGroup();
    void AddSpawner(ArenaPoint, Orientation);
    std::shared_ptr<BaseReferer> GetMouseBrain()            {return brain_;}
    
    void Specify()                                          override;
    void DeSpecify()                                        override {} //?set whoWillSpawn to zero?
    void TimeIsNow(int t, std::shared_ptr<Arena> arena)     override;

//    std::weak_ptr<Arena> arena_;
    std::vector<std::shared_ptr<MouseSpawner> > spawns_;
    std::shared_ptr<BaseReferer> brain_;
    int whoWillSpawn_;
};

class CatSpawnGroup : public SpawnGroup
{
public:
    CatSpawnGroup();
    void AddSpawner(ArenaPoint, Orientation, TimeRange timeRange, int planNum);

    void Specify()                                          override;
    void DeSpecify()                                        override {} //?disable all timeExacts?
    void TimeIsNow(int t, std::shared_ptr<Arena> arena)     override;
    
//    std::weak_ptr<Arena> arena_;
    std::vector<std::shared_ptr<CatSpawner> > spawns_;
};









//also GoalSpawner with bool willSpawn;



#endif	/* PUPPET_HPP */
