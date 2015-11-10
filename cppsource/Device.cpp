/* 
 * File:   Device.cpp
 * Author: Dormouse
 *
 * Created on 24 April 2015, 15:09
 */

#include "Device.hpp"

#include "ChipPlan.hpp"

sf::Vector2f Device::GetWorldPos() const
{
    sf::Vector2f planPos { 0.0f, 0.0f };
    sf::Vector2f devicePos;
    //If subplans are to be visible when viewing parent plans then
    //later versions will need to factor in the accumulated plan offsets.
//        sf::Vector2f devicePos = planPos + mapGridToCoords(GetPosInPlan());
    auto p = container.lock();
    if (p) { devicePos = p->MapGridToCoords(GetPosInPlan()); }
    return devicePos;
}
