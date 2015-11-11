/* 
 * File:   Pansions.cpp
 * Author: Dormouse
 * 
 * Created on 11 November 2015, 19:49
 */

#include "Pansions.hpp"
#include <cassert>

int Pansions::MapDumbToSmart(int dumb) const
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
    
    auto it = pansions.begin();
    while (it != pansions.end() and not done)
    {
        //Only positive number expansions matter...
        if (dumb >= 0 and it->first >= 0) {
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
        //Or only negative number expansions matter...
        if (dumb < 0 and it->first < 0) {
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
    if (not done and dumb >= 0) smart = dumb - totalAdditions;
    if (not done and dumb <  0) smart = dumb - totalAdditions;
    return smart;
}

int Pansions::MapSmartToDumb(int smart) const
{
    int totalAdditions = 0;
    for (auto p: pansions) {
        if (p.first > 0 and p.first < smart) {
            totalAdditions += p.second - 1;
        }
        if (p.first < 0 and p.first >= smart) {
            totalAdditions -= p.second - 1;
        }
    }
    return smart + totalAdditions;
}

int Pansions::GetSize(int smart) const
{
    if (pansions.count(smart))
    {
        return pansions.at(smart);
    }
    else return 1;
}

void Pansions::Insert(int smart, int size)
{
    assert(size > 0);
    pansions.erase(smart);
    if (size != 1) {
        pansions.insert( {smart, size} );
    }
}
