/* 
 * File:   Model.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:53
 */

#include "Model.hpp"
#include <iostream>

BaseReferer::BaseReferer()
{
}

void BaseReferer::Logic()
{
    basePlan->PassOnCalculate();
    basePlan->PassOnAct();
}

void BaseReferer::SetPlan(std::shared_ptr<ChipPlan> p)
{
    basePlan = p;
}

void BaseReferer::StepOut(bool charge, int slot)
{
    ;
}
void BaseReferer::SetModified()
{
    ;
}
void BaseReferer::SwapIn(std::shared_ptr<ChipPlan> p)
{
    auto smart_this = basePlan->GetReferer();
    basePlan = p;
    p->RegisterReferer( smart_this );
}







Model::Model()
    :serializer(std::make_shared<Serializer>())
    ,userData(std::make_shared<UserData>(serializer))
    ,baseReferer(std::make_shared<BaseReferer>())
{
    serializer->LoadUserData(userData);
    
    auto basePlan = BlobFactory::MakePlan();
    basePlan->RegisterReferer(baseReferer);
    baseReferer->SetPlan(basePlan);
}

void Model::Logic()
{
    baseReferer->Logic();
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
            if (ref) ref->SwapIn(emptyPlan);
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
                if (ref) ref->SwapIn(loadedPlan);
                return loadedPlan;
            }
        }
    }
    return nullptr;
}

void Model::SavePlan(PlanPos pos)
{
    if (not pos.IsLocated()) 
    {
        auto plan = pos.GetPlan();
        int oldID = plan->GetPlanID();
        std::string oldName = userData->GetNameByID( oldID );
        serializer->SavePlan(plan, userData);
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
void Model::SavePlanAsNew(PlanPos pos)
{
    if (not pos.IsLocated()) 
    {
        auto plan = pos.GetPlan();
        int oldID = plan->GetPlanID();
        serializer->SavePlan(plan, userData);
        userData->AddAncestryEntry(plan->GetPlanID(), oldID);
        userData->AddAutoName(plan->GetPlanID());
    }
}
