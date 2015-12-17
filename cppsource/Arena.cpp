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
    mots.emplace_back(guy);
}
void Arena::RegisterMotile(std::shared_ptr<ArenaMotile> guy)
{
    mots.emplace_back(guy);
}


bool Arena::IsInBounds(ArenaPoint xy)
{
    return ( xy.x >= minCorner.x and xy.y >= minCorner.y and xy.x <= maxCorner.x and xy.y <= maxCorner.y );
}

bool Arena::WhiskerDetect(ArenaPoint xy)
{
    if (not IsInBounds(xy)) return true;
    for (auto &x : mots)
    {
        if ( x->GetPos() == xy and x->WhiskerDetectable() )
        {
            return true;
        }
    }
    return false;
}


