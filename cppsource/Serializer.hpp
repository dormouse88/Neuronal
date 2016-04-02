/* 
 * File:   Serializer.hpp
 * Author: Dormouse
 *
 * Created on 23 April 2015, 12:44
 */

#ifndef SERIALIZER_HPP
#define	SERIALIZER_HPP

#include <memory>
#include <vector>
#include <set>
#include "../external/pugixml-1.6/src/pugixml.hpp"
#include "BlobFactory.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "ChipPlan.hpp"
class PlanGroupData; //fwd dec
class Arena; //fwd dec

class Serializer
{
public:
    Serializer();

    void LoadLevel(LevelNum num, std::shared_ptr<Arena> a, std::shared_ptr<BlobFactory> f);
    
    //Plan...
    PlanID    GetFirstFreePlanID() const;
    bool      SaveUserPlan(PlanShp);
    PlanShp   LoadUserPlan(PlanID, std::shared_ptr<BlobFactory>);
//    PlanShp   LoadLevelPlan(LevelNum, PlanID, std::shared_ptr<BlobFactory>);

    //PlanGroups...
    void LoadPlanGroupData(std::shared_ptr<PlanGroupData>);
    void SavePlanGroupData(std::shared_ptr<PlanGroupData>);
    
private:
    void OpenFile(pugi::xml_document & doc, const char * fileName);
    pugi::xml_node GetNameNodeByID(PlanID);
    pugi::xml_node GetNameNodeByName(std::string);

    bool DoSavePlan(pugi::xml_node container, PlanShp);
    PlanShp LoadPlanRecursively(pugi::xml_node container, PlanID id, std::shared_ptr<BlobFactory> factory);
    
    pugi::xml_document userDoc_;
    pugi::xml_document levelsDoc_;
};

#endif	/* SERIALIZER_HPP */


