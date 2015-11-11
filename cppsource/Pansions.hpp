/* 
 * File:   Pansions.hpp
 * Author: Dormouse
 *
 * Created on 11 November 2015, 19:49
 */

#ifndef PANSIONS_HPP
#define	PANSIONS_HPP

#include <map>

class Pansions
{
public:
    int MapSmartToDumb(int point) const;
    int MapDumbToSmart(int dumb) const;
    int GetSize(int smart) const;
    void Insert(int smart, int size);
private:
    std::map<int, int> pansions;
};

#endif	/* PANSIONS_HPP */

