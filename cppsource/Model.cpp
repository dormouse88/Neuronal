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
    ,factory_(std::make_shared<BlobFactory>(userData))
    ,arena(std::make_shared<Arena>(factory_))
{
    serializer->LoadUserData(userData);
    serializer->LoadLevel(1, arena, factory_);
    arena->Specify();
}

void Model::OuterTick()
{
    arena->TimeAdvance();
}
void Model::InnerTick()
{
    arena->GetMouseBrain()->TickOnce();
}

std::shared_ptr<ChipPlan> Model::WipePlan(PlanPos pos, bool forced)
{
    if (not pos.IsLocated())
    {
        std::shared_ptr<ChipPlan> plan = pos.GetPlan();
        if (forced or not plan->IsModified())
        {
            auto emptyPlan = factory_->MakePlan();
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
            auto loadedPlan = serializer->LoadUserPlan(num, factory_);
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
        if ( serializer->SaveUserPlan(plan) )
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
        if ( serializer->SaveUserPlan(plan) )
        {
            userData->AddAncestryEntry(plan->GetPlanID(), oldID);
            userData->AddAutoName(plan->GetPlanID());
        }
    }
}
