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
#include "../external/pugixml-1.6/src/pugixml.hpp"
#include "BlobFactory.hpp"
#include "Device.hpp"
#include "Wire.hpp"
#include "ChipPlan.hpp"

class Serializer
{
public:
    Serializer();
    ~Serializer() {}

    std::shared_ptr<ChipPlan> LoadPlan(int planID);
    void SavePlan(std::shared_ptr<ChipPlan> plan);
    void SavePlanAsNew(std::shared_ptr<ChipPlan> plan);

    std::string GetName(int planID);
    void RemoveName(int planID);
    void AddName(int planID, std::string name);
    
private:
    void AddAutoName(int planID);
    pugi::xml_node GetNodeByID(int planID);
    pugi::xml_node GetNodeByName(std::string name);
    
    void OpenFile(pugi::xml_document & doc, const char * fileName);
    
    void SavePlanParts(std::shared_ptr<ChipPlan> plan_p);
    std::shared_ptr<ChipPlan> LoadPlanParts(int planID);

    pugi::xml_document doc;
};

#endif	/* SERIALIZER_HPP */

