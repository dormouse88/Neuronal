/* 
 * File:   Arena.cpp
 * Author: Dormouse
 * 
 * Created on 16 December 2015, 22:49
 */

#include "Arena.hpp"
#include "Puppet.hpp"

bool operator==(const ArenaPoint & lhs, const ArenaPoint & rhs)
{
    return (lhs.x == rhs.x and lhs.y == rhs.y);
}

Arena::Arena()
    :minCorner(-7, -7)
    ,maxCorner(7, 7)
{}

void Arena::RegisterStatic(std::shared_ptr<ArenaStatic> guy)
{
    statics.emplace_back(guy);
}
void Arena::RegisterMotile(std::shared_ptr<ArenaMotile> guy)
{
    motiles.emplace_back(guy);
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
    
    for (auto &x: statics) x->Draw(rt);
    for (auto &x: motiles) x->Draw(rt);
}



bool Arena::IsInBounds(ArenaPoint xy)
{
    return ( xy.x >= minCorner.x and xy.y >= minCorner.y and xy.x <= maxCorner.x and xy.y <= maxCorner.y );
}

void Arena::Interactions()
{
    //collision checking between guys...
    for (auto &x: motiles)
    {
        if (not IsInBounds(x->GetActualPos())) x->Die();
        if (x->GetPredationStatus() == PredationStatus::MOUSE)
        {
            for (auto &y: statics)
            {
                if (x->GetActualPos() == y->GetActualPos()) x->Die();
            }
            for (auto &y: motiles)
            {
                if (y->GetActualPos() == x->GetActualPos() and y->GetPredationStatus() == PredationStatus::CAT) x->Die();
            }
        }
    }
    //need to check for goal collisions too.
}

bool Arena::WhiskerDetect(ArenaPoint xy)
{
    //Should whiskers detect out-of-bounds???
    if (not IsInBounds(xy)) return true;
    //Should check for statics too here probably
    for (auto &x : motiles)
    {
        if ( x->GetActualPos() == xy and x->WhiskerDetectable() )
        {
            return true;
        }
    }
    return false;
}


