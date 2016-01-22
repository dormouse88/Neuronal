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
#include "BasicTypes.hpp"


class Model
{
public:
    Model();
    Model(const Model&) = delete;

    void OuterTick();
    void InnerTick();
    
    Shp<Arena> GetArena()                                       {return arena;} //for View
    Shp<BaseReferer> GetMouseBrain()                            {return arena->GetMouseBrain(); }

    PlanShp WipePlan(PlanShp, bool forced);
    PlanShp LoadPlan(PlanShp, PlanNav nav);
    void SavePlan(PlanShp);
    void SavePlanAsNew(PlanShp);

    PlanShp EngageNameFilter(PlanShp plan, std::string filter)        { userData->SetNameFilter(filter); return LoadPlan(plan, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                               { return userData->GetNameFilter(); }
    
    bool CanAddName(int planID) const                       {userData->CanAddName(planID);}
    void AddName(int id, std::string name)                  {userData->AddName(id, name); }
    void RemoveName(int id)                                 {userData->RemoveName(id);    }

    Shp<const UserData> GetUserData() const     {return userData;}
    Shp<BlobFactory> GetFactory()               {return factory_;}
private:
    Shp<Serializer> serializer;
    Shp<UserData> userData;
    Shp<BlobFactory> factory_;

    Shp<Arena> arena;
};


#endif	/* MODEL_HPP */

