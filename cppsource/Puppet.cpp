/* 
 * File:   Puppet.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:05
 */

#include "Puppet.hpp"
#include <cassert>

//senses
const std::string L_WHISK = "L_WHISK";
const std::string F_WHISK = "F_WHISK";

//actions
const std::string L_FOOT = "L_FOOT";
const std::string R_FOOT = "R_FOOT";

Puppet::Puppet(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :ArenaMotile(s_pos, s_ori, ar)
    ,inner(std::make_shared<BaseReferer>())
{
    std::shared_ptr<ChipPlan> basePlan = BlobFactory::MakePlan();
    inner->SetSubPlan(basePlan, inner);

    InitBrain();
}

void Puppet::InitBrain()
{
    std::vector<std::string> ins;
    ins.push_back(L_WHISK);
    ins.push_back(F_WHISK);
    std::vector<std::string> outs;
    outs.push_back(L_FOOT);
    outs.push_back(R_FOOT);
    inner->DefineXputs(ins, outs);
}

void Puppet::Act()
{
    auto outs = inner->RetrieveOutputs();

    //Perform an action based on the output information from the brain...
    auto lamb = [&outs] (std::string n) { return outs.count(n) > 0 and outs.at(n) == true; };
    if (lamb(L_FOOT) and lamb(R_FOOT)) {
        Forward();
    }
    else if (lamb(L_FOOT)) {
        Left();
    }
    else if (lamb(R_FOOT)) {
        Right();
    }
}

void Puppet::Sense()
{
    //sends the new sense data to the brain from the arena...
    auto arenaLock = arena.lock();
    inner->SetInputState( L_WHISK, arenaLock->WhiskerDetect( GetPos(OriEnums::LEFT) ) );
    inner->SetInputState( F_WHISK, arenaLock->WhiskerDetect( GetPos(OriEnums::FRONT) ) );
}



//////////////////////////////////////ArenaStatic....
ArenaStatic::ArenaStatic(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :pos(s_pos)
    ,ori(s_ori)
    ,arena(ar)
{}






//////////////////////////////////////ArenaMotile....
ArenaMotile::ArenaMotile(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :ArenaStatic(s_pos, s_ori, ar)
{}

void ArenaMotile::Forward()
{
    //set pos according to ori...
    pos =  GetPos(OriEnums::FRONT);
    //do bounds checking...
    BoundsCheck();
}
void ArenaMotile::Left()
{
    ori--;
    if (ori < 0) ori = 3;
}
void ArenaMotile::Right()
{
    ori++;
    if (ori > 3) ori = 0;
}
void ArenaMotile::Die()
{
    ;
}
ArenaPoint ArenaMotile::GetPos(Orientation tile)
{
    if      ( (ori + tile) % 4 == 0) return ArenaPoint { pos.x, pos.y - 1 };
    else if ( (ori + tile) % 4 == 1) return ArenaPoint { pos.x + 1, pos.y };
    else if ( (ori + tile) % 4 == 2) return ArenaPoint { pos.x, pos.y + 1 };
    else if ( (ori + tile) % 4 == 3) return ArenaPoint { pos.x - 1, pos.y };
    else assert(false);
}
void ArenaMotile::BoundsCheck()
{
    if (not arena.lock()->IsInBounds( pos ))
    {
        Die();
    }
}




////////////////////////////////////////
//Hero
void Hero::Draw(sf::RenderTarget & rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {210,140,140} );
    if (ori == 0 or ori == 2) box.setSize( sf::Vector2f{ 70.f, 100.f } );
    else box.setSize( sf::Vector2f{ 100.f, 70.f } );
    box.setPosition( sf::Vector2f{ -1600.f + pos.x * 160.f, pos.y * 160.f } );

    rt.draw(box);
}
//Cat
void Cat::Draw(sf::RenderTarget & rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {255,0,0} );
    if (ori == 0 or ori == 2) box.setSize( sf::Vector2f{ 70.f, 100.f } );
    else box.setSize( sf::Vector2f{ 100.f, 70.f } );
    box.setPosition( sf::Vector2f{ -1600.f + pos.x * 160.f, pos.y * 160.f } );

    rt.draw(box);
}


std::shared_ptr<Cat> SpawnCat(ArenaPoint p, Orientation o, std::shared_ptr<Arena> arena)
{
    auto guy = std::make_shared<Cat>(p,o,arena);
    arena->RegisterMotile(guy);
    return guy;
}
std::shared_ptr<Hero> SpawnHero(ArenaPoint p, Orientation o, std::shared_ptr<Arena> arena)
{
    auto guy = std::make_shared<Hero>(p,o,arena);
    arena->RegisterMotile(guy);
    return guy;
}
