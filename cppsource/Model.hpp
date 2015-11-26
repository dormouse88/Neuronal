/* 
 * File:   Model.hpp
 * Author: Dormouse
 *
 * Created on 16 January 2015, 11:53
 */

#ifndef MODEL_HPP
#define	MODEL_HPP

#include <memory>
#include "BlobFactory.hpp"
#include "Serializer.hpp"
#include "UserData.hpp"
#include "ChipPlan.hpp"

class BaseReferer : public RefererInterface
{
public:
    BaseReferer();
    void Logic();
    void SetPlan(std::shared_ptr<ChipPlan> p);
    virtual void StepOut(bool charge, int slot) override;
    virtual void SetModified() override;
    virtual void SwapIn(std::shared_ptr<ChipPlan>) override;

//private:
    std::shared_ptr<ChipPlan> basePlan;    
};


class Model
{
public:
    Model();
    Model(const Model&) = delete;

    void Logic();
    
    //void SetBasePlan(std::shared_ptr<ChipPlan> newPlan)         {baseReferer->basePlan = newPlan;}
    const std::shared_ptr<ChipPlan> GetBasePlan() const                     {return baseReferer->basePlan;}

    std::shared_ptr<ChipPlan> WipePlan(PlanPos, bool forced);
    std::shared_ptr<ChipPlan> LoadPlan(PlanPos, PlanNav nav);
    void SavePlan(PlanPos pos);
    void SavePlanAsNew(PlanPos pos);

    std::shared_ptr<ChipPlan> SetNameFilter(PlanPos pos, std::string filter)        { userData->SetNameFilter(filter); return LoadPlan(pos, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                               { return userData->GetNameFilter(); }

    
    void AddName(int id, std::string name)                  {userData->AddName(id, name); }
    void RemoveName(int id)                                 {userData->RemoveName(id);    }
private:
    std::shared_ptr<Serializer> serializer;
    std::shared_ptr<UserData> userData;
    std::shared_ptr<BaseReferer> baseReferer;
};


#endif	/* MODEL_HPP */

