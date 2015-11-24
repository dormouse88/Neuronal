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

//ancestry
    std::shared_ptr<const Relatives> GetRelatives(int id) const;
    //setters
    void AddAncestryEntry(int id, int anc);

//names    
    int GetIDByName(std::string name) const;
    std::string GetNameByID(int planID) const;
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
    
    std::shared_ptr<Serializer> serializer;
    
    friend class Serializer;
};

#endif	/* USERDATA_HPP */

