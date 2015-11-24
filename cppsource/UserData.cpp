/* 
 * File:   UserData.cpp
 * Author: Dormouse
 * 
 * Created on 24 November 2015, 15:05
 */

#include "UserData.hpp"
#include <cassert>

UserData::UserData() {
}


const Relatives& UserData::GetRelatives(int id)
{
    try {
        return ancestry.at(id);
    }
    catch (std::out_of_range & e)
    {
        //whatevs
    }
}


void UserData::AddAncestryEntry(int id, int anc)
{
    //Note that map::insert() won't do anything if key is already present
    Relatives r;
    assert(id > 0);
    {
        ancestry.insert( std::make_pair(id, r) );
        ancestry.at(id).parent = anc;
    }
    if (anc > 0) 
    {
        ancestry.insert( std::make_pair(anc, r) );
        ancestry.at(anc).kids.insert(id);
    }
}


