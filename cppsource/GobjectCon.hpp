/* 
 * File:   GobjectCon.hpp
 * Author: Dormouse
 *
 * Created on 14 April 2015, 23:18
 */

#ifndef GOBJECTCON_HPP
#define	GOBJECTCON_HPP

class GobjectCon
{
public:
    GobjectCon() {}
    virtual bool IsDead() const = 0;
    virtual ~GobjectCon() {}
};


#endif	/* GOBJECTCON_HPP */

