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
#include "Arena.hpp"
#include "Puppet.hpp"


class Model
{
public:
    Model();
    Model(const Model&) = delete;

    void Logic();
    
    std::shared_ptr<Arena> GetArena()                                       {return arena;} //for View
    std::shared_ptr<BaseReferer> GetMouseBrain()                            {return arena->GetMouseBrain(); }

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

    std::shared_ptr<Arena> arena;
};


#endif	/* MODEL_HPP */

