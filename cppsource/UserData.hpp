/* 
 * File:   UserData.hpp
 * Author: Dormouse
 *
 * Created on 24 November 2015, 15:05
 */

#ifndef USERDATA_HPP
#define	USERDATA_HPP

struct Relatives
{
    Relatives() :parent(0) {}
    int parent;
    std::set<int> kids;
};

class UserData
{
public:
    UserData();

    const Relatives& GetRelatives(int id);

    void AddAncestryEntry(int id, int anc);

private:
    std::map<int, Relatives> ancestry;

};

#endif	/* USERDATA_HPP */

