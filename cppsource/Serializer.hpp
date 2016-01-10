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
class UserData; //fwd dec
class Arena; //fwd dec

class Serializer
{
public:
    Serializer();

    void LoadLevel(int num, std::shared_ptr<Arena> a, std::shared_ptr<BlobFactory> f);
    
    //Model interface...
    bool                        SaveUserPlan(std::shared_ptr<ChipPlan> plan_p);
    std::shared_ptr<ChipPlan>   LoadUserPlan(int planID, std::shared_ptr<BlobFactory>);
    std::shared_ptr<ChipPlan>   LoadLevelPlan(int levelNum, int planID, std::shared_ptr<BlobFactory>);

    //UserData interface...
    void LoadUserData(std::shared_ptr<UserData>);
    void SaveAddAncestryEntry(int id, int anc);
    void SaveRemoveName(int planID);
    void SaveAddName(int planID, std::string name);

private:
    void OpenFile(pugi::xml_document & doc, const char * fileName);
    pugi::xml_node GetNameNodeByID(int planID);
    pugi::xml_node GetNameNodeByName(std::string name);

    bool SavePlanRecursively(pugi::xml_node container, std::shared_ptr<ChipPlan> plan_p);
    std::shared_ptr<ChipPlan> LoadPlanRecursively(pugi::xml_node container, int planID, std::shared_ptr<BlobFactory> factory);
    
    pugi::xml_document userDoc_;
    pugi::xml_document levelsDoc_;
};

#endif	/* SERIALIZER_HPP */


