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

class Serializer
{
public:
    Serializer();
    
    //Model interface...
    void SavePlan(std::shared_ptr<ChipPlan> plan_p, std::shared_ptr<UserData>);
    std::shared_ptr<ChipPlan> LoadPlan(int planID, std::shared_ptr<UserData>);

    //UserData interface...
    void LoadUserData(std::shared_ptr<UserData>);
    void SaveAddAncestryEntry(int id, int anc);
    void SaveRemoveName(int planID);
    void SaveAddName(int planID, std::string name);

private:
    void OpenFile(pugi::xml_document & doc, const char * fileName);
    pugi::xml_node GetNameNodeByID(int planID);
    pugi::xml_node GetNameNodeByName(std::string name);
    
    pugi::xml_document doc;
};

#endif	/* SERIALIZER_HPP */


