/* 
 * File:   GobjectView.hpp
 * Author: Dormouse
 *
 * Created on 09 April 2015, 03:33
 */

#ifndef GOBJECTVIEW_HPP
#define	GOBJECTVIEW_HPP

class GobjectView
{
public:
    GobjectView() {}
    virtual bool IsDead() const = 0;
    virtual ~GobjectView() {}
};


#endif	/* GOBJECTVIEW_HPP */

