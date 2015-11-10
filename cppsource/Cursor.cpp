/* 
 * File:   Cursor.cpp
 * Author: Dormouse
 * 
 * Created on 18 January 2015, 20:37
 */

#include "Cursor.hpp"

Cursor::Cursor(sf::Color color)
    :active(false), m_pos(sf::Vector2i{0, 0})
{
    representation.setFillColor(sf::Color::Transparent);
    representation.setOutlineColor( color );
    representation.setOutlineThickness(2.5f);
    representation.setSize( GRID_SIZE );
}

void Cursor::Draw(sf::RenderTarget & rt)
{
    if (active) {
        representation.setPosition( GetWorldPos() );
        rt.draw(representation, sf::RenderStates(sf::BlendAdd));
    }
}

sf::Vector2f Cursor::GetWorldPos() const
{
    if (!active) throw;
    auto p = m_plan.lock();
    if (p) return p->MapGridToCoords(m_pos);
    throw;
}
sf::Vector2i Cursor::GetGridPos() const
{
    if (!active) throw;
    return m_pos;
}
void Cursor::SetWorldPos(sf::Vector2f worldPos, std::shared_ptr<ChipPlan> plan)
{
    auto b = plan->GetPaddedBound();
    if (b)
    {
        if (b->contains(worldPos))
        {
            m_plan = plan;
            m_pos = plan->MapCoordsToGrid(worldPos);
            active=true;
        }
        else active=false;
    }
}
