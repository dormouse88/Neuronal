/* 
 * File:   Arena.cpp
 * Author: Dormouse
 * 
 * Created on 16 December 2015, 22:49
 */

#include "Arena.hpp"
#include "Puppet.hpp"

Arena::Arena()
    :minCorner(-7, -7)
    ,maxCorner(7, 7)
    ,isTimeOn_(true)
    ,t_(-6)
{
    InitBoard();
    mouseSpawnGroup.Specify();
    catSpawnGroup.Specify();
}

//Have an Arena Serializer that has access to Arena and calls all the necessary methods to create all these things.
//Decide an XML schema for defining levels.
//
//A "level" Specifies:
//Arena Dimensions
//MouseXPuts
//MouseSpawners
//CatPlans -- how about this?
//CatSpawners
//GoalSpawners

void Arena::InitBoard()
{
    MakeMouseSpawner(ArenaPoint{2,0}, 0);
    MakeCatSpawner(ArenaPoint{2,3}, 0, TimeRange{-5,-3}, 9);
}

void Arena::TimeAdvance()
{
    mouseSpawnGroup.TimeIsNow(t_, shared_from_this() );
    catSpawnGroup.TimeIsNow(t_, shared_from_this() );
    //act
    //interact
    //sense
    for (auto &cat: cats)
        cat->Act();
    Interactions();
    for (auto &cat: cats)
        cat->Sense();

    for (auto &mouse: mice)
        mouse->Act();
    Interactions();
    for (auto &mouse: mice)
        mouse->Sense();
    t_++;
}

void Arena::RegisterMouse(std::shared_ptr<Mouse> guy)
{
    mice.emplace_back(guy);
}
void Arena::RegisterCat(std::shared_ptr<Cat> guy)
{
    cats.emplace_back(guy);
}
void Arena::MakeMouseSpawner(ArenaPoint p, Orientation o)
{
    mouseSpawnGroup.AddSpawner(p, o);
}
void Arena::MakeCatSpawner(ArenaPoint p, Orientation o, TimeRange timeRange, int planNum)
{
    catSpawnGroup.AddSpawner(p, o, timeRange, planNum);
}


void Arena::Draw(sf::RenderTarget &rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {35,35,75} );
    box.setOutlineColor( sf::Color {155,155,255} );
    box.setOutlineThickness(3.f);
    box.setSize( sf::Vector2f{ ARENA_GRID_SIZE.x * (maxCorner.x - minCorner.x), ARENA_GRID_SIZE.y * (maxCorner.y - minCorner.y) } );
    box.setPosition( sf::Vector2f{ -1600.f + (minCorner.x * ARENA_GRID_SIZE.x), (minCorner.y * ARENA_GRID_SIZE.y) } );

    rt.draw(box);
    
    for (auto &x: cats) x->Draw(rt);
    for (auto &x: mice) x->Draw(rt);
}



bool Arena::IsInBounds(ArenaPoint xy)
{
    return ( xy.x >= minCorner.x and xy.y >= minCorner.y and xy.x <= maxCorner.x and xy.y <= maxCorner.y );
}

void Arena::Interactions()
{
    //collision checking between guys...
    for (auto &x: cats)
    {
        if (not IsInBounds(x->GetActualPos())) x->Die();
    }
    for (auto &x: mice)
    {
        if (not IsInBounds(x->GetActualPos())) x->Die();
        for (auto &y: cats)
        {
            if (x->GetActualPos() == y->GetActualPos()) x->Die();
        }
    }
    //need to check for goal collisions too.
}

bool Arena::WhiskerDetect(ArenaPoint xy)
{
    //Should whiskers detect out-of-bounds???
    if (not IsInBounds(xy)) return true;
    for (auto &x : cats)
    {
        if ( x->GetActualPos() == xy and x->WhiskerDetectable() )
        {
            return true;
        }
    }
    return false;
}


