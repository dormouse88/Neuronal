/* 
 * File:   SpawnGroups.cpp
 * Author: Dormouse
 *
 * Created on 12 January 2016, 13:07
 */

#include "SpawnGroups.hpp"
#include "Arena.hpp" //fwd dec

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

void MouseSpawnGroup::TimeIsNow(OTime t, std::shared_ptr<Arena> arena)
{
    if (t == 0 and whoWillSpawn_ != -1)
    {
        auto chosenOne = spawns_.at(whoWillSpawn_);
        auto guy = std::make_shared<Mouse>(chosenOne->pos_, chosenOne->ori_, arena, brain_);
        arena->RegisterMouse(guy);
    }
}

void MouseSpawnGroup::Draw(sf::RenderTarget &rt, std::shared_ptr<Arena> ar)
{
    sf::ConvexShape shape;
    {
        // resize it to 5 points
        shape.setPointCount(5);
        // define the points
        const int LONG = 100;
        const int SHORT = 75;
        shape.setPoint(0, sf::Vector2f(0, -LONG));
        shape.setPoint(1, sf::Vector2f(SHORT, -SHORT));
        shape.setPoint(2, sf::Vector2f(SHORT, SHORT));
        shape.setPoint(3, sf::Vector2f(-SHORT, SHORT));
        shape.setPoint(4, sf::Vector2f(-SHORT, -SHORT));
    }
    
    shape.setOutlineThickness( -3.f );
    shape.setFillColor( sf::Color::Transparent );
    shape.setOutlineColor( MOUSE_COLOR_1 );
    for (auto s: spawns_)
    {
        auto fr = ar->GetCellBounds(s->pos_.x,s->pos_.y);
        shape.setPosition( fr.left + (fr.width/2.f), fr.top + (fr.height/2.f) );
        shape.setRotation( 90.f * s->ori_ );
        rt.draw(shape);
    }
}




//CatSpawnGroup
CatSpawnGroup::CatSpawnGroup(std::shared_ptr<BlobFactory> factory)
    :factory_(factory)
{}

std::shared_ptr<CatSpawner> CatSpawnGroup::AddSpawner(ArenaPoint p, Orientation o, TimeRange timeRange, PlanID planNum)
{
    auto sp = std::make_shared<CatSpawner>(p, o, timeRange, planNum, factory_);
    spawns_.emplace_back(sp);
    return sp;
}

OTime CatSpawnGroup::CalculateEarliestTime() const
{
    OTime et = 0;
    for (auto &c: spawns_) {
        if (c->timeExact_ < et) et = c->timeExact_;
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

void CatSpawnGroup::TimeIsNow(OTime t, std::shared_ptr<Arena> arena)
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

void CatSpawnGroup::Draw(sf::RenderTarget &rt, std::shared_ptr<Arena> ar)
{
    sf::Text timeText;
    {
        timeText.setFont(ViewResources::GetInstance().font);
        timeText.setColor(CAT_COLOR_1);
        timeText.setCharacterSize(40);
    }
    sf::Text planText;
    {
        planText.setFont(ViewResources::GetInstance().font);
        planText.setColor(CAT_COLOR_1);
        planText.setCharacterSize(40);
    }
    sf::ConvexShape shape;
    {
        // resize it to 5 points
        shape.setPointCount(5);
        // define the points
        const int MULT = 20;
        const int USUAL = MULT * 3;
        const int EXTRA = MULT * 1;
        shape.setPoint(0, sf::Vector2f(0, -USUAL -EXTRA));
        shape.setPoint(1, sf::Vector2f(USUAL, -USUAL));
        shape.setPoint(2, sf::Vector2f(USUAL, USUAL));
        shape.setPoint(3, sf::Vector2f(-USUAL, USUAL));
        shape.setPoint(4, sf::Vector2f(-USUAL, -USUAL));

        shape.setOutlineThickness( -3.f );
        shape.setFillColor( sf::Color::Transparent );
        shape.setOutlineColor( CAT_COLOR_1 );
    }
    for (auto s: spawns_)
    {
        auto fr = ar->GetCellBounds(s->pos_.x,s->pos_.y);
        shape.setPosition( fr.left + (fr.width/2.f), fr.top + (fr.height/2.f) );
        shape.setRotation( 90.f * s->ori_ );
        rt.draw(shape);
        timeText.setString( patch::to_string( s->timeExact_ ) );
        timeText.setPosition( fr.left , fr.top );
        rt.draw(timeText);
        planText.setString( patch::to_string( s->planNum_ ) );
        planText.setPosition( fr.left + fr.width - planText.getGlobalBounds().width , fr.top );
        rt.draw(planText);
    }
}

