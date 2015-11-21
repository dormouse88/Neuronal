/* 
 * File:   Model.cpp
 * Author: Dormouse
 * 
 * Created on 16 January 2015, 11:53
 */

#include "Model.hpp"
#include <iostream>

Model::Model()
{
    basePlan = BlobFactory::MakePlan();
}

void Model::Logic()
{
    basePlan->PassOnCalculate();
    basePlan->PassOnAct();
}

void Model::StepOut(bool charge, int slot)
{
    ;
}
void Model::SetModified()
{
    ;
}
void Model::SwapIn(std::shared_ptr<ChipPlan> p)
{
    auto smart_this = basePlan->GetReferer();
    SetBasePlan(p);
    p->RegisterReferer( smart_this );
}




std::shared_ptr<ChipPlan> Model::WipePlan(std::shared_ptr<ChipPlan> plan, bool forced)
{
    if (forced or not plan->IsModified())
    {
        auto emptyPlan = BlobFactory::MakePlan();
        auto ref = plan->GetReferer();
        if (ref) ref->SwapIn(emptyPlan);
        return emptyPlan;
    }
    return nullptr;
}
std::shared_ptr<ChipPlan> Model::LoadPlan(int num, std::shared_ptr<ChipPlan> plan)
{
    if (not plan->IsModified())
    {
        auto loadedPlan = serializer.LoadFile(num);
        if (loadedPlan)
        {
            auto ref = plan->GetReferer();
            if (ref) ref->SwapIn(loadedPlan);
            return loadedPlan;
        }
    }
    return nullptr;
}

void Model::SavePlan(PlanPos pos)
{
    if (not pos.IsLocated()) serializer.SaveFile(pos.GetPlan());
}