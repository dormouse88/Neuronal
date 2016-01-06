/* 
 * File:   Puppet.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:05
 */

#include "Puppet.hpp"
#include <cassert>


//////////////////////////////////////ArenaStatic....
ArenaStatic::ArenaStatic(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :initial_pos(s_pos)
    ,initial_ori(s_ori)
    ,arena(ar)
{}






//////////////////////////////////////ArenaMotile....
ArenaMotile::ArenaMotile(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :ArenaStatic(s_pos, s_ori, ar)
    ,actual_pos(s_pos)
    ,actual_ori(s_ori)
{}

void ArenaMotile::WalkForward()
{
    actual_pos =  GetAdjacentPos(OriEnums::FRONT);
}
void ArenaMotile::TurnLeft()
{
    actual_ori--;
    if (actual_ori < 0) actual_ori = 3;
}
void ArenaMotile::TurnRight()
{
    actual_ori++;
    if (actual_ori > 3) actual_ori = 0;
}
void ArenaMotile::Die()
{
    actual_pos = GetInitialPos(); //(temporary solution)
}
ArenaPoint ArenaMotile::GetAdjacentPos(Orientation tile)
{
    if      ( (actual_ori + tile) % 4 == 0) return ArenaPoint { actual_pos.x,       actual_pos.y - 1 };
    else if ( (actual_ori + tile) % 4 == 1) return ArenaPoint { actual_pos.x + 1,   actual_pos.y };
    else if ( (actual_ori + tile) % 4 == 2) return ArenaPoint { actual_pos.x,       actual_pos.y + 1 };
    else if ( (actual_ori + tile) % 4 == 3) return ArenaPoint { actual_pos.x - 1,   actual_pos.y };
    else assert(false);
}







//////////////////////////// Puppet
//senses
const std::string F_WHISK = "F_WHISK";
const std::string L_WHISK = "L_WHISK";
const std::string R_WHISK = "R_WHISK";
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
        WalkForward();
    }
    else if (lamb(L_FOOT)) {
        TurnLeft();
    }
    else if (lamb(R_FOOT)) {
        TurnRight();
    }
}

void Puppet::Sense()
{
    //sends the new sense data to the brain from the arena...
    auto arenaLock = arena.lock();
    inner->SetInputState( F_WHISK, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::FRONT) ) );
    inner->SetInputState( L_WHISK, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::LEFT) ) );
    inner->SetInputState( R_WHISK, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::RIGHT) ) );
}












////////////////////////////////////////
//Hero
void Hero::Draw(sf::RenderTarget & rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {210,140,140} );
    if (GetActualOri() == 0 or GetActualOri() == 2) box.setSize( sf::Vector2f{ 70.f, 100.f } );
    else box.setSize( sf::Vector2f{ 100.f, 70.f } );
    box.setPosition( sf::Vector2f{ -1600.f + GetActualPos().x * ARENA_GRID_SIZE.x, GetActualPos().y * ARENA_GRID_SIZE.y } );

    rt.draw(box);
}
//Cat
void Cat::Draw(sf::RenderTarget & rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {255,0,0} );
    if (GetActualOri() == 0 or GetActualOri() == 2) box.setSize( sf::Vector2f{ 70.f, 100.f } );
    else box.setSize( sf::Vector2f{ 100.f, 70.f } );
    box.setPosition( sf::Vector2f{ -1600.f + GetActualPos().x * ARENA_GRID_SIZE.x, GetActualPos().y * ARENA_GRID_SIZE.y } );

    rt.draw(box);
}





//non-member Spawn Functions...
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
