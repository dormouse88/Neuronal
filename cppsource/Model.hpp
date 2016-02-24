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
#include "PlanGroupData.hpp"
#include "ChipPlan.hpp"
#include "Arena.hpp"
#include "Puppet.hpp"
#include "BasicTypes.hpp"

enum class PlanNamingMode { TRANSFER, AUTONAME, ANON };

class Model
{
public:
    Model();
    Model(const Model&) = delete;

    void OuterTick();
    void InnerTick();
    
    Shp<Arena> GetArena()                                       {return arena;} //for View
    Shp<BaseReferer> GetMouseBrain()                            {return arena->GetMouseBrain(); }

    PlanShp LoadPlan(PlanShp, PlanNav nav, bool forced = false);
    void SavePlan(PlanShp, PlanNamingMode);

    PlanShp EngageNameFilter(PlanShp plan, std::string filter)        { planGroupData_->SetNameFilter(REAL_NAME_PREFIX + filter); return LoadPlan(plan, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                 { return planGroupData_->GetNameFilter(); }
    std::string GetCleanRealPlanName(PlanID id) const;
    void SetRealName(PlanID id, std::string name);
    void RemoveName(PlanID id);

    Shp<const PlanGroupData> GetPlanGroupData() const       {return planGroupData_;}
    Shp<BlobFactory> GetFactory()                           {return factory_;}
private:
    void SavePlanRecursively(PlanShp, PlanNamingMode);
    
    Shp<Serializer> serializer;
    Shp<PlanGroupData> planGroupData_;
    Shp<BlobFactory> factory_;

    Shp<Arena> arena;
};


#endif	/* MODEL_HPP */

