/* 
 * File:   SpawnGroups.hpp
 * Author: Dormouse
 *
 * Created on 12 January 2016, 13:07
 */

#ifndef SPAWNGROUPS_HPP
#define	SPAWNGROUPS_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "ArenaBasics.hpp"
#include "Spawners.hpp"
#include "BlobFactory.hpp"
class Arena;  //fwd dec

class SpawnGroup
{
public:
    virtual void Specify() = 0;
    virtual void DeSpecify() = 0;
    virtual void TimeIsNow(int t, std::shared_ptr<Arena> arena) = 0;
    virtual void Draw(sf::RenderTarget &rt, std::shared_ptr<Arena> ar) = 0;
};


class MouseSpawnGroup : public SpawnGroup
{
public:
    MouseSpawnGroup(std::shared_ptr<BlobFactory>);
    std::shared_ptr<MouseSpawner> AddSpawner(ArenaPoint, Orientation);
    void DefineMouseXPuts(XPutFilter f)                     {brain_->DefineXputs(GetAllXPuts(), f);}
    std::shared_ptr<BaseReferer> GetMouseBrain()            {return brain_;}
    
    void Specify()                                          override;
    void DeSpecify()                                        override {} //?set whoWillSpawn to zero?
    void TimeIsNow(int t, std::shared_ptr<Arena> arena)     override;
    void Draw(sf::RenderTarget &rt, std::shared_ptr<Arena> ar) override;

private:
    std::vector<std::shared_ptr<MouseSpawner> > spawns_;
    std::shared_ptr<BaseReferer> brain_;
    int whoWillSpawn_;
};


class CatSpawnGroup : public SpawnGroup
{
public:
    CatSpawnGroup(std::shared_ptr<BlobFactory>);
    std::shared_ptr<CatSpawner> AddSpawner(ArenaPoint, Orientation, TimeRange timeRange, int planNum);
    int CalculateEarliestTime() const;

    void Specify()                                          override;
    void DeSpecify()                                        override {} //?disable all timeExacts?
    void TimeIsNow(int t, std::shared_ptr<Arena> arena)     override;
    void Draw(sf::RenderTarget &rt, std::shared_ptr<Arena> ar) override;

private:
    std::vector<std::shared_ptr<CatSpawner> > spawns_;
    std::shared_ptr<BlobFactory> factory_;
};



//also GoalSpawner with bool willSpawn;



#endif	/* SPAWNGROUPS_HPP */

