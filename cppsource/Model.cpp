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
    :baseReferer(std::make_shared<BaseReferer>())
{
    auto basePlan = BlobFactory::MakePlan();
    basePlan->RegisterReferer(baseReferer);
    baseReferer->SetPlan(basePlan);
}

void Model::Logic()
{
    baseReferer->Logic();
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
        auto loadedPlan = serializer.LoadPlan(num);
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
    if (not pos.IsLocated()) serializer.SavePlan(pos.GetPlan());
}
void Model::SavePlanAsNew(PlanPos pos)
{
    if (not pos.IsLocated()) serializer.SavePlanAsNew(pos.GetPlan());
}
