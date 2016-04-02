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

enum class PlanNamingMode { PROVIDED, TRANSFER, ANON }; //AUTONAME,

class Model
{
public:
    Model();
    Model(const Model&) = delete;

    Shp<Arena> GetArena()                                       {return arena;} //for View

    PlanShp LoadPlan(PlanShp, PlanNav nav, bool forced = false);
    void SavePlan(PlanShp, PlanNamingMode, std::string provided = "");

    PlanShp EngageNameFilter(PlanShp plan, std::string filter)        { planGroupData_->SetNameFilter(filter); return LoadPlan(plan, PlanNav::FILTER_NAME); }
        //{ planGroupData_->SetNameFilter(REAL_NAME_PREFIX + filter); return LoadPlan(plan, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                 { return planGroupData_->GetNameFilter(); }
        //{ auto s = planGroupData_->GetNameFilter(); if (s.size()>0) return s.substr(1); else return s; }
    std::string GetCleanRealPlanName(PlanID id) const;
    void AddOrChangeName(PlanID id, std::string name);
    void RemoveName(PlanID id);

    Shp<const PlanGroupData> GetPlanGroupData() const       {return planGroupData_;}
    Shp<BlobFactory> GetFactory()                           {return factory_;}
private:
    void SavePlanRecursively(PlanShp, PlanNamingMode, std::string provided = "");
    
    Shp<Serializer> serializer;
    Shp<PlanGroupData> planGroupData_;
    Shp<BlobFactory> factory_;

    Shp<Arena> arena;
    Shp<ChipPlan> mouseBasePlan;
};


#endif	/* MODEL_HPP */

