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
{
    serializer->LoadPlanGroupData(planGroupData_);
    serializer->LoadLevel(1, arena, factory_);
    arena->Specify();
}

void Model::OuterTick()
{
    arena->TimeAdvance();
}
void Model::InnerTick()
//Does an InnerTick ONLY if it won't affect the Outer
{
    if ( not arena->GetMouseBrain()->IsAnyOutputOn() )
        arena->GetMouseBrain()->TickOnce();
}

PlanShp Model::LoadPlan(PlanShp plan, PlanNav nav, bool forced)
{
    if (forced or not plan->IsModified())
    {
        PlanID newID = planGroupData_->GetID(plan->GetPlanID(), nav);
        auto newPlan = factory_->MakePlan();
        if (newID != 0)
            newPlan = serializer->LoadUserPlan(newID, factory_);
        assert(newPlan);
        {
            auto ref = plan->GetReferer();
            if (ref) ref->SetSubPlan(newPlan, ref);
            return newPlan;
        }
    }
    return nullptr;
}

void Model::SavePlan(PlanShp plan, PlanSaveMode mode)
{
    if (plan->IsModified())
    {
        PlanID oldID = plan->GetPlanID();
        std::string oldName = planGroupData_->GetNameByID( oldID );
        if (oldName.empty())
            mode = PlanSaveMode::ASNEW; //Saving as UPDATE on a nameless plan should be disabled in the UI really, so an assert instead.
        std::string newName = (mode == PlanSaveMode::ASNEW) ? planGroupData_->GetUnusedAutoName() : oldName;
        bool success = serializer->SaveUserPlan(plan);
        assert(success);
        PlanID newID = plan->GetPlanID();
        planGroupData_->AddAncestryEntry(newID, oldID);
        //serializer->SaveAddAncestryEntry(newID, oldID);
        if (mode == PlanSaveMode::UPDATE)
        {
            planGroupData_->RemoveName(oldID);
            //serializer->SaveRemoveName(oldID);
        }
        planGroupData_->AddName(newID, newName);
        //serializer->SaveAddName(newID, newName);
        serializer->SavePlanGroupData(planGroupData_);
    }
}

