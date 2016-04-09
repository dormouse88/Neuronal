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

//const std::string REAL_NAME_PREFIX = "%";
//const std::string AUTO_NAME_PREFIX = "#";
// int NAME_MAX_LENGTH = 30;
//const int AUTO_NAME_MAX_LENGTH = NAME_MAX_LENGTH - 1; //to account for the @ identifier
//const std::string AUTO_NAME_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//enum class NameType { NONE, AUTO, REAL };

enum class PlanNav { EMPTY, PARENT, FIRST_CHILD, PREV_SIBLING, NEXT_SIBLING, PREV_ID, NEXT_ID, PREV_NAME, NEXT_NAME, FILTER_NAME, PREV_NAMED_ID, NEXT_NAMED_ID };
//enum class IDNav { PREV_ID, NEXT_ID };
//enum class NameNav { PREV_NAME, NEXT_NAME };

//struct PlanName
//{
//    PlanName(std::string r, int s) :root(r), suffix(s)    {}
//    std::string GetComboString() const                      { return root + patch::to_string(suffix); }
//    bool IsNull() const                                     { return suffix == 0; }
//    std::string root;
//    int suffix;
//};

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
//    PlanID   GetIDByName(PlanName name) const;
    PlanName GetNameByID(PlanID planID) const;
//    PlanName GetUnusedAutoName() const;
    //setters
    void SetName(PlanID planID, std::string name);
    void RemoveName(PlanID planID);

private:
    std::string AddUniqueifyingAppendage(std::string name);
    std::map<PlanID, std::shared_ptr<Relatives> > ancestry;
    std::map<PlanID, PlanName> namesByID;
    std::map<PlanName, PlanID> namesByName;

    std::string nameFilter;
    
    friend class Serializer;
};


#endif	/* PLANGROUPDATA_HPP */

