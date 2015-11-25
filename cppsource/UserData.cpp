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


int UserData::GetID(int planID, PlanNav nav)
{
    int target = planID;

    if (nav == PlanNav::PREV_NAME or nav == PlanNav::NEXT_NAME)  //access the name maps
    {
        auto nid_it = namesByID.find(planID);
        if (namesByID.empty()) //names is empty
        {
            target = 0;
        }
        else if (nid_it == namesByID.end()) //planID has no name
        {
            target = namesByName.begin()->second;
        }
        else
        {
            auto nn_it = namesByName.find( nid_it->second );
            assert(nn_it != namesByName.end());
            if (nav == PlanNav::PREV_NAME and nn_it != namesByName.begin()) --nn_it;
            if (nav == PlanNav::NEXT_NAME and nn_it != std::prev(namesByName.begin())) ++nn_it;
            target = nn_it->second;
        }
    }
    else  //access the ancestry maps
    {
        auto begin = ancestry.begin();
        auto end = ancestry.end();
        auto it = ancestry.find(planID);

        if (begin == end) //ancestry is empty
        {
            target = 0;
        }
        else if (it == end) //planID was not found in ancestry
        {
            //switch on nav
            if (nav == PlanNav::PREV_ID) target = begin->first;
            else if (nav == PlanNav::NEXT_ID) target = (--end)->first;
            else target = begin->first;
        }
        else if (nav == PlanNav::PREV_ID)
        {
            if (it != begin) --it;
            target = it->first;
        }
        else if (nav == PlanNav::NEXT_ID)
        {
            if (it != std::prev(end)) ++it;
            target = it->first;
        }
        else if (nav == PlanNav::PARENT)
        {
            if (it->second->parent > 0) target = it->second->parent;
            else target = it->first;
        }
        else if (nav == PlanNav::FIRST_CHILD)
        {
            if (not it->second->kids.empty()) target = *( it->second->kids.begin() );
            else target = it->first;
        }
        else if (nav == PlanNav::PREV_SIBLING or nav == PlanNav::NEXT_SIBLING)
        {
            auto par_it = ancestry.find(it->second->parent);
            if (par_it != end)
            {
                auto sib_it = par_it->second->kids.find(planID);
                assert(sib_it != par_it->second->kids.end());

                if (nav == PlanNav::PREV_SIBLING and sib_it != par_it->second->kids.begin()) --sib_it;
                if (nav == PlanNav::NEXT_SIBLING and sib_it != std::prev(par_it->second->kids.end())) ++sib_it;
                target = *sib_it ;
            }
            else {
                target = it->first;
            }
        }
        else assert(false);
    }
    return target;
}


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


