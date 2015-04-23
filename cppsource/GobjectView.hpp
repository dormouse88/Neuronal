/* 
 * File:   GobjectView.hpp
 * Author: Dormouse
 *
 * Created on 09 April 2015, 03:33
 */

#ifndef GOBJECTVIEW_HPP
#define	GOBJECTVIEW_HPP

#include "Gobject.hpp"

class GobjectView
{
public:
    GobjectView(const Gobject & gobject_p) :gobject_m(gobject_p) {}
    virtual bool IsDead() const = 0;
    virtual ~GobjectView() {}
private:
    const Gobject & gobject_m;
};


#endif	/* GOBJECTVIEW_HPP */

