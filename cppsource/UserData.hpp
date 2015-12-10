/* 
 * File:   UserData.hpp
 * Author: Dormouse
 *
 * Created on 24 November 2015, 15:05
 */

#ifndef USERDATA_HPP
#define	USERDATA_HPP

#include <map>
#include <string>
#include <memory>
#include "Serializer.hpp"

enum class PlanNav { PARENT, FIRST_CHILD, PREV_SIBLING, NEXT_SIBLING, PREV_ID, NEXT_ID, PREV_NAME, NEXT_NAME, FILTER_NAME };
//enum class IDNav { PREV_ID, NEXT_ID };
//enum class NameNav { PREV_NAME, NEXT_NAME };

struct Relatives
{
    Relatives() :parent(0) {}
    int parent;
    std::set<int> kids;
};

class UserData
{
public:
    UserData(std::shared_ptr<Serializer>);

    int GetID(int planID, PlanNav nav) const;
    void SetNameFilter(std::string s)               {nameFilter = s;}
    std::string GetNameFilter() const               {return nameFilter;}
    
//ancestry
    std::shared_ptr<const Relatives> GetRelatives(int id) const;
    //setters
    void AddAncestryEntry(int id, int anc);

//names    
    int GetIDByName(std::string name) const;
    std::string GetNameByID(int planID) const;
    bool CanAddName(int planID) const;
    bool CanAddName(int planID, std::string) const;
    //setters
    void RemoveName(int planID);
    void AddName(int planID, std::string name);
    void AddAutoName(int planID);

private:
    void MakeAncestryEntry(int id, int anc);
    std::string GetUnusedAutoName();

    std::map<int, std::shared_ptr<Relatives> > ancestry;
    std::map<int, std::string> namesByID;
    std::map<std::string, int> namesByName;

    bool nameFiltering;
    std::string nameFilter;
    
    std::shared_ptr<Serializer> serializer;
    
    friend class Serializer;
};

#endif	/* USERDATA_HPP */

