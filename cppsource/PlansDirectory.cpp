/* 
 * File:   PlansDirectory.cpp
 * Author: Dormouse
 * 
 * Created on 24 November 2015, 15:05
 */

#include "PlansDirectory.hpp"
#include <cassert>
#include <map>


PlansDirectory::PlansDirectory()
{}

PlanID PlansDirectory::GetID(PlanID planID, PlanNav nav) const
{
    //assert(planID != 0); //May receive zero
    PlanID target = planID;

    if (nav == PlanNav::EMPTY)
    {
        target = NULL_PID;
    }
    else if (nav == PlanNav::PREV_NAME or nav == PlanNav::NEXT_NAME or nav == PlanNav::FILTER_NAME)  //access the named by name maps
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
        }

        if (lower == upper) //(filtered)range is empty
        {
            target = NULL_PID;
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
    else if (nav == PlanNav::PREV_NAMED_ID or nav == PlanNav::NEXT_NAMED_ID)  //access the named by id maps
    {        //(this section does not currently apply the nameFilter)
        auto lower = namesByID.begin();
        auto upper = namesByID.end();
        auto valid_it = namesByID.lower_bound(planID);
        if (lower == upper)
            target = NULL_PID;
        else
        {
            if (valid_it->first == planID)
            {
                if (nav == PlanNav::PREV_NAMED_ID and valid_it != lower)
                    --valid_it;
                if (nav == PlanNav::NEXT_NAMED_ID and valid_it != std::prev(upper))
                    ++valid_it;
            }
            target = valid_it->first;
        }
    }
    else  //access the ancestry maps
    {
        auto begin = ancestry.begin();
        auto end = ancestry.end();
        auto it = ancestry.find(planID);

        if (begin == end) //ancestry is empty
        {
            target = NULL_PID;
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
            if (it->second->parent != NULL_PID) target = it->second->parent;
            else target = it->first;
        }
        else if (nav == PlanNav::FIRST_CHILD)
        {
            if (not it->second->kids.empty()) target = *( it->second->kids.begin() );
            else target = it->first;
        }
        else if (nav == PlanNav::PREV_SIBLING or nav == PlanNav::NEXT_SIBLING)
        {
            PlanID parentID = it->second->parent;
            if (ancestry.count(parentID) > 0)
            {
                auto & sibs = ancestry.at(parentID)->kids;
                auto sib_it = sibs.find(planID);
                if (sib_it == sibs.end()) sib_it = sibs.begin();
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
    //assert(target != 0); //May return zero
    return target;
}


//Ancestry //Getters
std::shared_ptr<const Relatives> PlansDirectory::GetRelatives(PlanID id) const
{
    if (ancestry.count(id) > 0) return ancestry.at(id);
    else return nullptr;
}

//Ancestry //Setters
void PlansDirectory::AddAncestryEntry(PlanID id, PlanID anc)
{
    assert(ancestry.count(id) == 0);
    //Note that map::insert() won't do anything if key is already present
    assert(id != NULL_PID);
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
//PlanID PlanGroupData::GetIDByName(PlanName name) const
//{
//    if (namesByName.count(name) > 0)
//        return namesByName.at(name);
//    else
//        return NULL_PID;
//}
PlanName PlansDirectory::GetNameByID(PlanID planID) const
{
    if (namesByID.count(planID) > 0)
        return namesByID.at(planID);
    else
        return NULL_PLAN_NAME;
}

//Names //Setters
void PlansDirectory::SetName(PlanID planID, PlanName name)
{
    if (planID != NULL_PID)
    {
        RemoveName(planID);
        std::string uniqueName = AddUniqueifyingAppendage(name);

        namesByID.insert( std::make_pair(planID, uniqueName) );
        namesByName.insert( std::make_pair(uniqueName, planID) );
    }
}

void PlansDirectory::RemoveName(PlanID planID)
{
    if (planID != NULL_PID)
    {
        namesByName.erase( GetNameByID(planID) );
        namesByID.erase(planID);
    }
}


//PlanName PlanGroupData::GetUnusedAutoName() const
//{
//    PlanName randomName;
//    for (int i = 0; i <= AUTO_NAME_MAX_LENGTH; ++i)
//    {
//        randomName.push_back( AUTO_NAME_CHARS.at( rand() % AUTO_NAME_CHARS.length() ) );
//        if (namesByName.count(AUTO_NAME_PREFIX + randomName) == 0)
//        {
//            return AUTO_NAME_PREFIX + randomName;
//        }
//    }
//    return NULL_PLAN_NAME;
//}

std::string PlansDirectory::AddUniqueifyingAppendage(std::string name)
{
    if (namesByName.count(name) == 0)
        return name;
    const std::string CHAR_POOL = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    name.append( 1, CHAR_POOL.front() );

    //Initializes tag to the first available value (similar to numeric counting).
    //These loops look a bit odd but work as intended. (A,B,C...Z,AA,AB...AZ,BA,BB,etc)
    while (true)
    {
        if (namesByName.count(name) == 0)
            return name;
        int ti = name.size() - 1;
        while ( (not (ti < 0)) and (name.at(ti) == CHAR_POOL.back()) )
        {
            name.at(ti) = CHAR_POOL.front();
            ti--;
        }
        if (ti < 0)
        {
            name.insert(0, 1, CHAR_POOL.front());
        }
        else
        {
            name.at(ti)++;
        }
    }
}
