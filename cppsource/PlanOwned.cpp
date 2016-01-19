/* 
 * File:   PlanOwned.cpp
 * Author: Dormouse
 * 
 * Created on 09 November 2015, 16:52
 */

#include "PlanOwned.hpp"

#include "ChipPlan.hpp"

//void PlanOwned::SetModified()
//{
//    auto c = container.lock();
//    if (c) c->SetModified();
//}

std::shared_ptr<ChipPlan> PlanOwned::GetContainer()
{
    return container.lock();
}