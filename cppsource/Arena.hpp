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
#include "SpawnGroups.hpp"

class Arena : public std::enable_shared_from_this<Arena>
{
public:
    Arena(Shp<BlobFactory>);
    void Specify();
    void TimeAdvance();
    Shp<BaseReferer> GetMouseBrain()    { return mouseSpawnGroup.GetMouseBrain(); }

    void RegisterMouse(Shp<Mouse> mouse);
    void RegisterCat(Shp<Cat> cat);
    Shp<MouseSpawner> MakeMouseSpawner(ArenaPoint p, Orientation o);
    Shp<CatSpawner> MakeCatSpawner(ArenaPoint p, Orientation o, TimeRange timeRange, int planNum);

    sf::FloatRect GetCellBounds(ArenaPoint ap);
    ArenaPoint GetArenaPoint( VectorWorld worldPos );
    Shp<ArenaEntity> GetEntity(ArenaPoint addr);
    
    void Draw(sf::RenderTarget &rt);
    bool IsInBounds(ArenaPoint);
    bool WhiskerDetect(ArenaPoint);
private:
    void Interactions();
    void CrossoverCheck();
    int levelNum;
    CatSpawnGroup catSpawnGroup;
    MouseSpawnGroup mouseSpawnGroup;
    std::vector< Shp<Cat> > cats;
    std::vector< Shp<Mouse> > mice;
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

