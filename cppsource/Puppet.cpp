/* 
 * File:   Puppet.cpp
 * Author: Dormouse
 * 
 * Created on 11 December 2015, 14:05
 */

#include "Puppet.hpp"
#include <cassert>
#include "Arena.hpp"

//////////////////////////////////////Entity....
ArenaEntity::ArenaEntity(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar)
    :pos_(s_pos)
    ,ori_(s_ori)
    ,arena_(ar)
    ,alive_(true)
{}

void ArenaEntity::WalkForward()
{
    pos_ =  GetAdjacentPos(OriEnums::FRONT);
}
void ArenaEntity::TurnLeft()
{
    ori_--;
    if (ori_ < 0) ori_ = 3;
}
void ArenaEntity::TurnRight()
{
    ori_++;
    if (ori_ > 3) ori_ = 0;
}
void ArenaEntity::Die()
{
    alive_ = false;
    pos_ = {0,0}; //GetInitialPos(); //(temporary solution)
}
ArenaPoint ArenaEntity::GetAdjacentPos(Orientation tile)
{
    if      ( (ori_ + tile) % 4 == 0) return ArenaPoint { pos_.x,       pos_.y - 1 };
    else if ( (ori_ + tile) % 4 == 1) return ArenaPoint { pos_.x + 1,   pos_.y };
    else if ( (ori_ + tile) % 4 == 2) return ArenaPoint { pos_.x,       pos_.y + 1 };
    else if ( (ori_ + tile) % 4 == 3) return ArenaPoint { pos_.x - 1,   pos_.y };
    else assert(false);
}







//////////////////////////// Puppet
Puppet::Puppet(ArenaPoint s_pos, Orientation s_ori, std::shared_ptr<Arena> ar, std::shared_ptr<BaseReferer> brain)
    :ArenaEntity(s_pos, s_ori, ar)
    ,brain_(brain)
{}

void Puppet::Act()
{
    auto outs = brain_->RetrieveOutputs();

    //Perform an action based on the output information from the brain...
    auto lamb = [&outs] (std::string n) { return outs.count(n) > 0 and outs.at(n) == true; };
    if (lamb(L_FOOT.name) and lamb(R_FOOT.name)) {
        WalkForward();
    }
    else if (lamb(L_FOOT.name)) {
        TurnLeft();
    }
    else if (lamb(R_FOOT.name)) {
        TurnRight();
    }
}

void Puppet::Sense()
{
    //sends the new sense data to the brain from the arena...
    auto arenaLock = arena_.lock();
    brain_->SetInputState( F_WHISK.name, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::FRONT) ) );
    brain_->SetInputState( L_WHISK.name, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::LEFT) ) );
    brain_->SetInputState( R_WHISK.name, arenaLock->WhiskerDetect( GetAdjacentPos(OriEnums::RIGHT) ) );
}












////////////////////////////////////////

//Mouse
void Mouse::Draw(sf::RenderTarget & rt)
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






//////////////////////////////////////Spawn....

//MouseSpawnGroup
MouseSpawnGroup::MouseSpawnGroup(std::shared_ptr<BlobFactory> factory)
    :brain_(factory->MakeBrain())
    ,whoWillSpawn_(-1)
{}

std::shared_ptr<MouseSpawner> MouseSpawnGroup::AddSpawner(ArenaPoint p, Orientation o)
{
    auto sp = std::make_shared<MouseSpawner>(p,o);
    spawns_.emplace_back(sp);
    return sp;
}

void MouseSpawnGroup::Specify()
{
    if (spawns_.size() > 0)
    {
        whoWillSpawn_ = std::rand() % spawns_.size();
    }
    else
    {
        whoWillSpawn_ = -1;
    }
}

void MouseSpawnGroup::TimeIsNow(int t, std::shared_ptr<Arena> arena)
{
    if (t == 0 and whoWillSpawn_ != -1)
    {
        auto chosenOne = spawns_.at(whoWillSpawn_);
        auto guy = std::make_shared<Mouse>(chosenOne->pos_, chosenOne->ori_, arena, brain_);
        arena->RegisterMouse(guy);
    }
}

//CatSpawnGroup
CatSpawnGroup::CatSpawnGroup(std::shared_ptr<BlobFactory> factory)
    :factory_(factory)
{}

std::shared_ptr<CatSpawner> CatSpawnGroup::AddSpawner(ArenaPoint p, Orientation o, TimeRange timeRange, int planNum)
{
    auto sp = std::make_shared<CatSpawner>(p, o, timeRange, planNum, factory_);
    spawns_.emplace_back(sp);
    return sp;
}

int CatSpawnGroup::CalculateEarliestTime() const
{
    int et = 0;
    for (auto &c: spawns_) {
        if (c->timeRange_.start < et) et = c->timeRange_.start;
    }
    return et;
}

void CatSpawnGroup::Specify()
{
    for (auto &cat: spawns_)
    {
        int rangeSize = cat->timeRange_.end +1 - cat->timeRange_.start;
        cat->timeExact_ = cat->timeRange_.start + ( std::rand() % rangeSize );
    }
}

void CatSpawnGroup::TimeIsNow(int t, std::shared_ptr<Arena> arena)
{
    for (auto &x: spawns_)
    {
        if (t == x->timeExact_)
        {
            auto guy = std::make_shared<Cat>(x->pos_, x->ori_, arena, x->brain_);
            arena->RegisterCat(guy);
        }
    }
}


