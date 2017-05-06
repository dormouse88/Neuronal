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
#include "PlansDirectory.hpp"
#include "ChipPlan.hpp"
#include "Arena.hpp"
//#include "Puppet.hpp"
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

    PlanShp EngageNameFilter(PlanShp plan, std::string filter)        { plansDir_->SetNameFilter(filter); return LoadPlan(plan, PlanNav::FILTER_NAME); }
    std::string GetNameFilter() const                                 { return plansDir_->GetNameFilter(); }

    PlanName GetPlanName(PlanID id) const;
    void AddName(PlanID id, std::string name);
    void ChangeName(PlanID id, std::string name);
    void RemoveName(PlanID id);

    Shp<const PlansDirectory> GetPlansDirectory() const       {return plansDir_;}
    Shp<BlobFactory> GetFactory()                           {return factory_;}
private:
    void SavePlanRecursively(PlanShp, PlanNamingMode, std::string provided = "");
    
    Shp<Serializer> serializer;
    Shp<PlansDirectory> plansDir_;
    Shp<BlobFactory> factory_;

    Shp<Arena> arena;
};


#endif	/* MODEL_HPP */

