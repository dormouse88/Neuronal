/* 
 * File:   Arena.hpp
 * Author: Dormouse
 *
 * Created on 16 December 2015, 22:49
 */

#ifndef ARENA_HPP
#define	ARENA_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "BlobFactory.hpp"
#include "ArenaBasics.hpp"
#include "Puppet.hpp"

const sf::Vector2f ARENA_GRID_SIZE { 200.f , 200.f };

class Arena : public std::enable_shared_from_this<Arena>
{
public:
    Arena(std::shared_ptr<BlobFactory>);
    void Specify();
    void TimeAdvance();
    std::shared_ptr<BaseReferer> GetMouseBrain()    { return mouseSpawnGroup.GetMouseBrain(); }

    void RegisterMouse(std::shared_ptr<Mouse> mouse);
    void RegisterCat(std::shared_ptr<Cat> cat);
    std::shared_ptr<MouseSpawner> MakeMouseSpawner(ArenaPoint p, Orientation o);
    std::shared_ptr<CatSpawner> MakeCatSpawner(ArenaPoint p, Orientation o, TimeRange timeRange, int planNum);

    void Draw(sf::RenderTarget &rt);
    bool IsInBounds(ArenaPoint);
    bool WhiskerDetect(ArenaPoint);
private:
    void Interactions();
    int levelNum;
    CatSpawnGroup catSpawnGroup;
    MouseSpawnGroup mouseSpawnGroup;
    std::vector< std::shared_ptr<Cat> > cats;
    std::vector< std::shared_ptr<Mouse> > mice;
    ArenaPoint minCorner;
    ArenaPoint maxCorner;
    bool isTimeOn_;
    TimeExact t_;

    friend class Serializer;
};

//potential optimization:
    //Store ArenaStatic keyed by location such as std::multimap< ArenaPoint, std::shared_ptr<ArenaStatic> > stats;
    //Store ArenaMotile in a dumb container such as std::vector< std::shared_ptr<ArenaMotile> > mots;


#endif	/* ARENA_HPP */

