/* 
 * File:   Model.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:53
 */

#include "Model.hpp"
#include <iostream>


Model::Model()
    :serializer(std::make_shared<Serializer>())
    ,userData(std::make_shared<UserData>(serializer))
    ,arena(std::make_shared<Arena>())
    ,hero( SpawnHero(ArenaPoint{2,0}, 0, arena) )
    ,cat1( SpawnCat(ArenaPoint{2,3}, 0, arena) )
{
    serializer->LoadUserData(userData);
}

void Model::Logic()
{
    //act
    //interact
    //sense
    hero->Act();
    cat1->Act();
    arena->Interactions();
    hero->Sense();
    cat1->Sense();
}

std::shared_ptr<ChipPlan> Model::WipePlan(PlanPos pos, bool forced)
{
    if (not pos.IsLocated())
    {
        std::shared_ptr<ChipPlan> plan = pos.GetPlan();
        if (forced or not plan->IsModified())
        {
            auto emptyPlan = BlobFactory::MakePlan();
            auto ref = plan->GetReferer();
            if (ref) ref->SetSubPlan(emptyPlan, ref);
            return emptyPlan;
        }
    }
    return nullptr;
}
std::shared_ptr<ChipPlan> Model::LoadPlan(PlanPos pos, PlanNav nav)
{
    if (not pos.IsLocated())
    {
        std::shared_ptr<ChipPlan> plan = pos.GetPlan();
        if (not plan->IsModified())
        {
            int num = userData->GetID(plan->GetPlanID(), nav);
            auto loadedPlan = serializer->LoadPlan(num, userData);
            if (loadedPlan)
            {
                auto ref = plan->GetReferer();
                if (ref) ref->SetSubPlan(loadedPlan, ref);
                return loadedPlan;
            }
        }
    }
    return nullptr;
}

void Model::SavePlan(PlanPos pos)
{
    auto plan = pos.GetPlan();
    if (not pos.IsLocated() and plan->IsModified())
    {
        int oldID = plan->GetPlanID();
        std::string oldName = userData->GetNameByID( oldID );
        if ( serializer->SavePlan(plan, userData) )
        {
            userData->AddAncestryEntry(plan->GetPlanID(), oldID);
            if (not oldName.empty()) {
                userData->RemoveName(oldID);
                userData->AddName(plan->GetPlanID(), oldName);
            }
            else {
                userData->AddAutoName(plan->GetPlanID());
            }
        }
    }
}
void Model::SavePlanAsNew(PlanPos pos)
{
    auto plan = pos.GetPlan();
    if (not pos.IsLocated() and plan->IsModified())
    {
        int oldID = plan->GetPlanID();
        if ( serializer->SavePlan(plan, userData) )
        {
            userData->AddAncestryEntry(plan->GetPlanID(), oldID);
            userData->AddAutoName(plan->GetPlanID());
        }
    }
}
