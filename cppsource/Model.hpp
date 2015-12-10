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
    
    //RefererInterface...
    virtual void StepOut(bool charge, int slot) override;
    virtual void SetModified() override;
    virtual void SetSubPlan(std::shared_ptr<ChipPlan>, std::shared_ptr<RefererInterface>) override;
    virtual std::shared_ptr<ChipPlan> GetSubPlan() override;
    
private:
    std::shared_ptr<ChipPlan> basePlan;    
};


class Model
{
public:
    Model();
    Model(const Model&) = delete;

    void Logic();
    
    //void SetBasePlan(std::shared_ptr<ChipPlan> newPlan)         {baseReferer->basePlan = newPlan;}
    const std::shared_ptr<ChipPlan> GetBasePlan() const                     {return baseReferer->GetSubPlan();}

    std::shared_ptr<ChipPlan> WipePlan(PlanPos, bool forced);
    std::shared_ptr<ChipPlan> LoadPlan(PlanPos, PlanNav nav);
    void SavePlan(PlanPos pos);
    void SavePlanAsNew(PlanPos pos);

    std::shared_ptr<ChipPlan> SetNameFilter(PlanPos pos, std::string filter)        { userData->SetNameFilter(filter); return LoadPlan(pos, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                               { return userData->GetNameFilter(); }
    
    bool CanAddName(int planID) const                       {userData->CanAddName(planID);}
    void AddName(int id, std::string name)                  {userData->AddName(id, name); }
    void RemoveName(int id)                                 {userData->RemoveName(id);    }

    std::shared_ptr<const UserData> GetUserData() const     {return userData;}
private:
    std::shared_ptr<Serializer> serializer;
    std::shared_ptr<UserData> userData;
    std::shared_ptr<BaseReferer> baseReferer;
};


#endif	/* MODEL_HPP */

