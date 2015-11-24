/* 
 * File:   UserData.cpp
 * Author: Dormouse
 * 
 * Created on 24 November 2015, 15:05
 */

#include "UserData.hpp"
#include <cassert>
#include <map>

const int NAME_MAX_LENGTH = 30;
const std::string AUTO_NAME_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


UserData::UserData(std::shared_ptr<Serializer> s)
    :serializer(s)
{}

//Ancestry //Getters
std::shared_ptr<const Relatives> UserData::GetRelatives(int id) const
{
    if (ancestry.count(id) > 0) return ancestry.at(id);
    else return nullptr;
}

//Ancestry //Setters
void UserData::AddAncestryEntry(int id, int anc)
{
    MakeAncestryEntry(id, anc);
    serializer->SaveAddAncestryEntry(id, anc);
}
void UserData::MakeAncestryEntry(int id, int anc)
{
    //Note that map::insert() won't do anything if key is already present
    auto r = std::make_shared<Relatives>();
    assert(id > 0);
    {
        ancestry.insert( std::make_pair(id, r) );
        ancestry.at(id)->parent = anc;
    }
    if (anc > 0) 
    {
        ancestry.insert( std::make_pair(anc, r) );
        ancestry.at(anc)->kids.insert(id);
    }
}


///////////////////////////////////////////////////////////////////


//Names //Getters
int UserData::GetIDByName(std::string name) const
{
    if (namesByName.count(name) > 0) return namesByName.at(name);
    else return 0;
}

std::string UserData::GetNameByID(int planID) const
{
    if (namesByID.count(planID) > 0) return namesByID.at(planID);
    else return "";
}

//Names //Setters
void UserData::RemoveName(int planID)
{
    namesByName.erase( GetNameByID(planID) );
    namesByID.erase(planID);
    serializer->SaveRemoveName(planID);
}
void UserData::AddName(int planID, std::string name)
{
    namesByID.insert( std::make_pair(planID, name) );
    namesByName.insert( std::make_pair(name, planID) );
    serializer->SaveAddName(planID, name);
}
void UserData::AddAutoName(int planID)
{
    AddName(planID, GetUnusedAutoName());
}





//////////////////////////////////////////////////////////////////////////PRIVATE

std::string UserData::GetUnusedAutoName()
{
    std::string randomName;
    for (int i = 0; i <= NAME_MAX_LENGTH; ++i)
    {
        randomName.push_back( AUTO_NAME_CHARS.at( rand() % AUTO_NAME_CHARS.length() ) );
        if (namesByName.count(randomName) == 0)
        {
            return randomName;
        }
    }
    throw "NAME MAX LENGTH Reached! (highly unlikely to reach this)";
}


