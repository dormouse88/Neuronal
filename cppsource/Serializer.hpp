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
#include "UserData.hpp"

class Serializer
{
public:
    Serializer();

    std::shared_ptr<ChipPlan> LoadPlan(int planID);
    void SavePlan(std::shared_ptr<ChipPlan> plan);
    void SavePlanAsNew(std::shared_ptr<ChipPlan> plan);

    int GetIDByName(std::string name);
    std::string GetNameByID(int planID);
    void RemoveName(int planID);
    void AddName(int planID, std::string name);

private:
    std::string GetUnusedAutoName();
    pugi::xml_node GetNameNodeByID(int planID);
    pugi::xml_node GetNameNodeByName(std::string name);
    
    void OpenFile(pugi::xml_document & doc, const char * fileName);

    void SavePlanParts(std::shared_ptr<ChipPlan> plan_p);
    std::shared_ptr<ChipPlan> LoadPlanParts(int planID);

    pugi::xml_document doc;
    std::shared_ptr<UserData> userData;
};

#endif	/* SERIALIZER_HPP */

