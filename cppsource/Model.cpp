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
    ,planGroupData_(std::make_shared<PlanGroupData>())
    ,factory_(std::make_shared<BlobFactory>(planGroupData_))
    ,arena(std::make_shared<Arena>(factory_))
    ,mouseBasePlan(factory_->MakePlan())
{
    serializer->LoadPlanGroupData(planGroupData_);
    serializer->LoadLevel(1, arena, factory_);
    auto mouseBrain = arena->GetMouseBrain();
    mouseBrain->SetSubPlan(mouseBasePlan, mouseBrain);
    arena->Specify();
}

PlanShp Model::LoadPlan(PlanShp plan, PlanNav nav, bool forced)
{
    if (forced or not plan->IsModified())
    {
        PlanID newID = planGroupData_->GetID(plan->GetPlanID(), nav);
        auto newPlan = factory_->MakePlan();
        if (newID != NULL_PID)
            newPlan = serializer->LoadUserPlan(newID, factory_);
        assert(newPlan);
        {
            auto ref = plan->GetReferer();
            if (ref) ref->SetSubPlan(newPlan, ref);
            newPlan->StructuralRefresh();
            return newPlan;
        }
    }
    return nullptr;
}

void Model::SavePlan(PlanShp plan, PlanNamingMode mode, std::string provided)
{
    if (plan->IsModified())
    {
        SavePlanRecursively(plan, mode, provided);
        serializer->SavePlanGroupData(planGroupData_);
    }
}

void Model::SavePlanRecursively(PlanShp plan, PlanNamingMode mode, std::string provided)
{
    for (auto d: plan->devices)
    {
        auto h = std::dynamic_pointer_cast<ChipHandle>(d);
        if (h)
        {
            auto p = h->GetSubPlan();
            if (p and p->IsModified())
            {
                SavePlanRecursively(p, PlanNamingMode::ANON);
            }
        }
    }
    PlanID oldID = plan->GetPlanID();
    PlanID newID = serializer->GetFirstFreePlanID();

    //update the plan in memory with new IDs...
    plan->ancID = oldID;
    plan->planID = newID;
    
    serializer->SaveUserPlan(plan);
    planGroupData_->AddAncestryEntry(newID, oldID);

    //names...
    PlanName oldName = planGroupData_->GetNameByID( oldID );
    if (mode == PlanNamingMode::PROVIDED)
    {
        AddName(newID, provided);
            //Will append unique suffix in event of name clash (or if empty string)
    }
//    else if (mode == PlanNamingMode::AUTONAME)
//    {
//        planGroupData_->AddName(newID, planGroupData_->GetUnusedAutoName(), false); //can assert success
//    }
    else if (mode == PlanNamingMode::TRANSFER)
    {
        assert( not (oldName.empty()) );
        RemoveName(oldID);
        AddName(newID, oldName);
    }
    else if (mode == PlanNamingMode::ANON)
        {    }
    else
        assert(false);
}


std::string Model::GetPlanName(PlanID id) const
{
    return planGroupData_->GetNameByID(id);
}

void Model::AddName(PlanID id, std::string name)
{
    if ( GetPlanName(id) == NULL_PLAN_NAME )
    {
        planGroupData_->SetName(id, name);
        serializer->SavePlanGroupData(planGroupData_);
    }
}

void Model::ChangeName(PlanID id, std::string name)
{
    if ( GetPlanName(id) != NULL_PLAN_NAME )
    {
        planGroupData_->SetName(id, name);
        serializer->SavePlanGroupData(planGroupData_);
    }
}

void Model::RemoveName(PlanID id)
{
    planGroupData_->RemoveName(id);
    serializer->SavePlanGroupData(planGroupData_);
}
