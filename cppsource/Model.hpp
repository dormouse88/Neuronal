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

enum class PlanSaveMode { UPDATE, ASNEW };

class Model
{
public:
    Model();
    Model(const Model&) = delete;

    void OuterTick();
    void InnerTick();
    
    Shp<Arena> GetArena()                                       {return arena;} //for View
    Shp<BaseReferer> GetMouseBrain()                            {return arena->GetMouseBrain(); }

    //PlanShp WipePlan(PlanShp, bool forced);
    PlanShp LoadPlan(PlanShp, PlanNav nav, bool forced = false);
    void SavePlan(PlanShp, PlanSaveMode);

    PlanShp EngageNameFilter(PlanShp plan, std::string filter)        { planGroupData_->SetNameFilter(filter); return LoadPlan(plan, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                 { return planGroupData_->GetNameFilter(); }
    
    bool CanAddName(PlanID id) const                            {planGroupData_->CanAddName(id);}
    void AddName(PlanID id, std::string name)                   {if (planGroupData_->CanAddName(id, name)) { planGroupData_->RemoveName(id); planGroupData_->AddName(id, name); serializer->SavePlanGroupData(planGroupData_); } }
    void RemoveName(PlanID id)                                  {planGroupData_->RemoveName(id); serializer->SavePlanGroupData(planGroupData_); }

    Shp<const PlanGroupData> GetPlanGroupData() const       {return planGroupData_;}
    Shp<BlobFactory> GetFactory()                           {return factory_;}
private:
    Shp<Serializer> serializer;
    Shp<PlanGroupData> planGroupData_;
    Shp<BlobFactory> factory_;

    Shp<Arena> arena;
};


#endif	/* MODEL_HPP */

