/* 
 * File:   Gobject.cpp
 * Author: Dormouse
 * 
 * Created on 09 November 2015, 16:52
 */

#include "Gobject.hpp"

#include "ChipPlan.hpp"

//void Gobject::SetModified()
//{
//    auto c = container.lock();
//    if (c) c->SetModified();
//}

std::shared_ptr<ChipPlan> Gobject::GetContainer()
{
    return container.lock();
}