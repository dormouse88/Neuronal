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

#include <iostream>
int UserData::GetID(int planID, PlanNav nav)
{
    int target = planID;

    if (nav == PlanNav::PREV_NAME or nav == PlanNav::NEXT_NAME or nav == PlanNav::FILTER_NAME)  //access the name maps
    {
        auto nn_it = namesByName.find( GetNameByID(planID) );
        auto nn_end = namesByName.end();

        auto lower = namesByName.begin();
        auto upper = namesByName.end();
        if (nameFilter.length() > 0)
        {
            lower = namesByName.lower_bound(nameFilter);
            char lastLetterPlusOne = static_cast<char>(nameFilter.back() + 1);
            std::string nameFilterPlusOne = nameFilter.substr(0, nameFilter.size()-1) + lastLetterPlusOne;
            upper = namesByName.lower_bound(nameFilterPlusOne);

            std::cout << "lower + upper: " << (lower == upper ? "SAME!" : "DFRNT") << std::endl;
            if (lower != nn_end) std::cout << "lower: " << lower->first << " and " << lower->second << std::endl;
            if (upper != nn_end) std::cout << "upper: " << upper->first << " and " << upper->second << std::endl;
        }

        if (lower == upper) //(filtered)range is empty
        {
            target = 0;
        }
        else if (nn_it == nn_end) //planID has no name
        {
            target = lower->second;
        }
        else if ( GetNameByID(planID).compare(0, nameFilter.length(), nameFilter) != 0 ) //filter is not a prefix of planID's name
        {
            target = lower->second;
        }
        else
        {
            assert(nn_it != upper);
            if ( nav == PlanNav::PREV_NAME and nn_it != lower ) --nn_it;
            if ( nav == PlanNav::NEXT_NAME and nn_it != std::prev(upper) ) ++nn_it;
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
            int parentID = it->second->parent;
            if (ancestry.count(parentID) > 0)
            {
                auto & sibs = ancestry.at(parentID)->kids;
                auto sib_it = sibs.find(planID);
                assert(sib_it != sibs.end());
                
                if (nav == PlanNav::PREV_SIBLING and sib_it != sibs.begin()) --sib_it;
                if (nav == PlanNav::NEXT_SIBLING and sib_it != std::prev(sibs.end())) ++sib_it;
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
    assert(ancestry.count(id) == 0);
    MakeAncestryEntry(id, anc);
    serializer->SaveAddAncestryEntry(id, anc);
}
void UserData::MakeAncestryEntry(int id, int anc)
{
    //Note that map::insert() won't do anything if key is already present
    assert(id > 0);
    {
        auto r = std::make_shared<Relatives>();
        ancestry.insert( std::make_pair(id, r) );
        ancestry.at(id)->parent = anc;
    }
    if (true)//(anc > 0)
    {
        auto r = std::make_shared<Relatives>();
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
    if ( GetNameByID(planID) != "")
    {
        namesByName.erase( GetNameByID(planID) );
        namesByID.erase(planID);
        serializer->SaveRemoveName(planID);
    }
}
void UserData::AddName(int planID, std::string name)
{
    if ( name != "" and GetNameByID(planID) == "" and GetIDByName(name) == 0 )
    {
        namesByID.insert( std::make_pair(planID, name) );
        namesByName.insert( std::make_pair(name, planID) );
        serializer->SaveAddName(planID, name);
    }
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
            return "~" + randomName;
        }
    }
    throw "NAME MAX LENGTH Reached! (highly unlikely to reach this)";
}


