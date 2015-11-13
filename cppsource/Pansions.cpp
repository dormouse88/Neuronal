/* 
 * File:   Pansions.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 19:49
 */

#include "Pansions.hpp"
#include <cassert>

Smart Pansions::MapDumbToSmart(Dumb dumb) const
{
    //Iterate thru the Pansions in order from zero
    //At each one, test if dumbInt can be reached without expanding it:
    //if (totalAdditions + first < dumbInt)
    //then smartInt = dumbInt - totalAdditions
    //Or Secondly, if it is inside that expansion:
    //else if (totalAdditions + first + (second-1) < dumbInt)
    //then smartInt = first
    //Or Thirdly, not at all:
    //else
    //then add (second-1) to the totalAdditions and go to next Pansion
    
    int totalAdditions = 0;
    bool done = false;
    int smart;
    
    if (dumb >= 0)
    {
        auto it = pansions.begin();
        while (it != pansions.end() and not done)
        {
            //Consider only the positive number expansions...
            if (it->first >= 0)
            {
                //prior to the expansion...
                if (totalAdditions + it->first > dumb) {
                    smart = dumb - totalAdditions;
                    done = true;
                }
                //inside the expansion...
                else if (totalAdditions + it->first + (it->second - 1) >= dumb) {
                    smart = it->first;
                    done = true;
                }
                //after the expansion...
                else {
                    totalAdditions += (it->second - 1);
                }
            }
            ++it;
        }
    }
    if (dumb < 0) //('else' basically)
    {
        auto it = pansions.rbegin();
        while (it != pansions.rend() and not done)
        {
            //Consider only the negative number expansions...
            if (it->first < 0)
            {
                //prior to the expansion...
                if (totalAdditions + it->first < dumb) {
                    smart = dumb - totalAdditions;
                    done = true;
                }
                //inside the expansion...
                else if (totalAdditions + it->first - (it->second - 1) <= dumb ) {
                    smart = it->first;
                    done = true;
                }
                //after the expansion...
                else {
                    totalAdditions -= (it->second - 1);
                }
            }
            ++it;
        }
    }
    
    if (not done and dumb >= 0) smart = dumb - totalAdditions;
    if (not done and dumb <  0) smart = dumb - totalAdditions;
    return smart;
}

Dumb Pansions::MapSmartToDumb(Smart smart) const
{
    int totalAdditions = 0;
    for (auto p: pansions) {
        if (p.first >= 0 and p.first < smart) {
            totalAdditions += p.second - 1;
        }
        if (p.first < 0 and p.first >= smart) {
            totalAdditions -= p.second - 1;
        }
    }
    return smart + totalAdditions;
}

Dumb Pansions::GetSize(Smart loc) const
{
    if (pansions.count(loc))
    {
        return pansions.at(loc);
    }
    else return 1;
}
#include <iostream>
void Pansions::SetSize(Smart loc, Dumb size)
{
    assert(size > 0);
    pansions.erase(loc);
    if (size != 1) {
        pansions.insert( {loc, size} );
    }
    std::cout << "Pans: " << this << std::endl;
    for (auto dbg: pansions) std::cout << dbg.first << "   " << dbg.second << std::endl;
}
