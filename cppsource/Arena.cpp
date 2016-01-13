/* 
 * File:   Arena.cpp
 * Author: Dormouse
 * 
 * Created on 16 December 2015, 22:49
 */

#include "Arena.hpp"
#include "Puppet.hpp"

const sf::Vector2f ARENA_GRID_SIZE  { 200.f , 200.f };
const sf::Vector2f ARENA_POS        { 1800.f, 0.f };

Arena::Arena(std::shared_ptr<BlobFactory> factory)
    :levelNum(0)
    ,mouseSpawnGroup(factory)
    ,catSpawnGroup(factory)
{}

void Arena::Specify()
{
    mouseSpawnGroup.Specify();
    catSpawnGroup.Specify();
    t_ = catSpawnGroup.CalculateEarliestTime() - 1;
}

void Arena::TimeAdvance()
{
    t_++;
    //act > spawn > interact > sense
    for (auto &cat: cats)
        cat->Act();
    CrossoverCheck();
    for (auto &mouse: mice)
        mouse->Act();
    mouseSpawnGroup.TimeIsNow(t_, shared_from_this() );
    catSpawnGroup.TimeIsNow(t_, shared_from_this() );
    Interactions();
    for (auto &cat: cats)
        cat->Sense();
    for (auto &mouse: mice)
        mouse->Sense();
}

void Arena::RegisterMouse(std::shared_ptr<Mouse> guy)
{
    mice.emplace_back(guy);
}
void Arena::RegisterCat(std::shared_ptr<Cat> guy)
{
    cats.emplace_back(guy);
}
std::shared_ptr<MouseSpawner> Arena::MakeMouseSpawner(ArenaPoint p, Orientation o)
{
    return mouseSpawnGroup.AddSpawner(p, o);
}
std::shared_ptr<CatSpawner> Arena::MakeCatSpawner(ArenaPoint p, Orientation o, TimeRange timeRange, int planNum)
{
    return catSpawnGroup.AddSpawner(p, o, timeRange, planNum);
}


void Arena::Draw(sf::RenderTarget &rt)
{
    sf::RectangleShape box;
    box.setFillColor( sf::Color {35,35,75} );
    box.setOutlineColor( sf::Color {155,155,255} );
    box.setOutlineThickness(3.f);
    auto min_c = GetCellBounds(minCorner.x, minCorner.y);
    auto max_c = GetCellBounds(maxCorner.x, maxCorner.y);
    box.setSize( sf::Vector2f { max_c.left - min_c.left + max_c.width, max_c.top - min_c.top + max_c.height } );
    box.setPosition( min_c.left, min_c.top );
    rt.draw(box);

    sf::RectangleShape dot;
    dot.setSize( {200.f, 200.f } );
    dot.setFillColor( sf::Color::Transparent );
    dot.setOutlineThickness(-2.f);
    dot.setOutlineColor( sf::Color {145, 145, 95} );
    for (int i = minCorner.x; i<=maxCorner.x; i++) {
        for (int j = minCorner.y; j<=maxCorner.y; j++) {
            auto b = GetCellBounds(i,j);
            dot.setPosition( b.left + (b.width*0.f), b.top + (b.height*0.f));
            rt.draw(dot);
        }
    }
    
    sf::Text timeText;
    timeText.setFont(ViewResources::GetInstance().font);
    timeText.setString( "t  =  " + patch::to_string( t_ ) );
    timeText.setColor(sf::Color::White);
    timeText.setCharacterSize(50.f);
    timeText.setPosition(ARENA_POS.x, ARENA_POS.y - 90.f);
    rt.draw(timeText);
    
    catSpawnGroup.Draw(rt, shared_from_this());
    mouseSpawnGroup.Draw(rt, shared_from_this());
    for (auto &x: cats) x->Draw(rt);
    for (auto &x: mice) x->Draw(rt);
}



bool Arena::IsInBounds(ArenaPoint xy)
{
    return ( xy.x >= minCorner.x and xy.y >= minCorner.y and xy.x <= maxCorner.x and xy.y <= maxCorner.y );
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

sf::FloatRect Arena::GetCellBounds(int x, int y)
{
    sf::FloatRect fr;
    fr.left = ARENA_POS.x + (ARENA_GRID_SIZE.x * (x - minCorner.x));
    fr.top = ARENA_POS.y + (ARENA_GRID_SIZE.y * (y - minCorner.y));
    fr.width = ARENA_GRID_SIZE.x;
    fr.height = ARENA_GRID_SIZE.y;
    return fr;
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

void Arena::CrossoverCheck()
{
    //Check against the special case of cat and mouse passing thru each other...
    for (auto &x: mice)
    {
        for (auto &y: cats)
        {
            if (x->GetActualPos() == y->GetActualPos() and
                x->GetActualOri() == (y->GetActualOri() + 2) % 4 )
                    x->Die();
        }
    }
}

