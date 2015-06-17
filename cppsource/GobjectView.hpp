/* 
 * File:   GobjectView.hpp
 * Author: Dormouse
 *
 * Created on 09 April 2015, 03:33
 */

#ifndef GOBJECTVIEW_HPP
#define	GOBJECTVIEW_HPP

#include <memory>
#include "Gobject.hpp"

#include <iostream>

class GobjectView
{
public:
    GobjectView(std::shared_ptr<const Gobject> gobject_p)
        :gobject_m(gobject_p)
    {}
    bool IsDead()
    {
        auto sh = gobject_m.lock(); std::cout << "-" << sh;
        if (sh) {
            std::cout << ">" << sh->IsDead();
            if (sh->IsDead() == false) return false;
        }
        return true;
    }
    virtual ~GobjectView() {}
private:
    std::weak_ptr<const Gobject> gobject_m;
};


#endif	/* GOBJECTVIEW_HPP */

