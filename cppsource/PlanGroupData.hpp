/* 
 * File:   PlanGroupData.hpp
 * Author: Dormouse
 *
 * Created on 24 November 2015, 15:05
 */

#ifndef PLANGROUPDATA_HPP
#define	PLANGROUPDATA_HPP

#include <map>
#include <set>
#include <string>
#include <memory>
#include <cstdlib>
#include "BasicTypes.hpp"

enum class PlanNav { EMPTY, PARENT, FIRST_CHILD, PREV_SIBLING, NEXT_SIBLING, PREV_ID, NEXT_ID, PREV_NAME, NEXT_NAME, FILTER_NAME };
//enum class IDNav { PREV_ID, NEXT_ID };
//enum class NameNav { PREV_NAME, NEXT_NAME };

struct Relatives
{
    Relatives() :parent(NULL_PID) {}
    PlanID parent;
    std::set<PlanID> kids;
};

class PlanGroupData
{
public:
    PlanGroupData();

    PlanID GetID(PlanID planID, PlanNav nav) const;
    void SetNameFilter(std::string s)               {nameFilter = s;}
    std::string GetNameFilter() const               {return nameFilter;}
    
//ancestry
    std::shared_ptr<const Relatives> GetRelatives(PlanID id) const;
    //setters
    void AddAncestryEntry(PlanID id, PlanID anc);

//names    
    PlanID   GetIDByName(PlanName name) const;
    PlanName GetNameByID(PlanID planID) const;
    PlanName GetUnusedAutoName() const;
    bool CanAddName(PlanID) const;
    bool CanAddName(PlanID, PlanName) const;
    //setters
    void RemoveName(PlanID planID);
    void AddName(PlanID planID, std::string name);

private:
    std::map<PlanID, std::shared_ptr<Relatives> > ancestry;
    std::map<PlanID, PlanName> namesByID;
    std::map<PlanName, PlanID> namesByName;

    bool nameFiltering;
    std::string nameFilter;
    
    friend class Serializer;
};

#endif	/* PLANGROUPDATA_HPP */

