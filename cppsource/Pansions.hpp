/* 
 * File:   Pansions.hpp
 * Author: Dormouse
 *
 * Created on 11 November 2015, 19:49
 */

#ifndef PANSIONS_HPP
#define	PANSIONS_HPP

#include <map>
#include "BasicTypes.hpp"

class Pansions
{
public:
    Smart MapDumbToSmart(Dumb dumb) const;
    Dumb MapSmartToDumb(Smart smart) const;
    Dumb GetSize(Smart loc) const;
    void SetSize(Smart loc, Dumb size);
private:
    std::map<int, int> pansions;
};

#endif	/* PANSIONS_HPP */

